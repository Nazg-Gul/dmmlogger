// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)
//
// Protocol description from http://perfec.to/ut60e/
//
// This table shows the bits used for most of the non-digit symbols:
//
//   Frame Offset  Sequence Id  0x08  0x04  0x02      0x01
//   0             0x01         AC    ?     Auto      RS232C
//   9             0x0a         u     n     k         ->|- (diode)
//   10            0x0b         m     %     M         *))) (beep)
//   11            0x0c         F     ohm   relative  Hold
//   12            0x0d         A     V     Hz        (Low Battery)
//   13            0x0e         ?     ?     ?         Celsius
//
// An encoded byte for a digit is assembled by taking the two low nibbles
// of adjacent data bytes, and shifting the lower-offset nibble left 4 bits.
//
//   encoded = (frame[1] << 4) | (frame[2] & 0xf)
//
// The four digits begin at offsets 1, 3, 5 and 7 within the frame.
// The negative sign is encoded in the high bit of the first digit,
// and the decimal points are encoded in the high bits of the other
// three digits.
//
//     Encoded Byte      Digit
// (with 0x80 cleared)
//
//   0x7d                0
//   0x05                1
//   0x5b                2
//   0x1f                3
//   0x27                4
//   0x3e                5
//   0x7e                6
//   0x15                7
//   0x7f                8
//   0x3f                9
//   0x00                Space
//   0x68                L
//

#include "ut60e.h"

#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <cassert>


UT60E::UT60E(const std::string &device)
  : SerialDMM(device) {
}

bool UT60E::Connect(void) {
  if (!SerialDMM::Connect()) {
    return false;
  }

  /* For some reason, the presence of RTS on the serial line
   * causes problems with data transmission, so this line
   * should be cut, or monitoring software should make sure
   * RTS is not asserted.
   *
   * See http://perfec.to/ut60e/
   */

  unsigned int mcr = 0;
  ioctl(file_descriptor_, TIOCMGET, &mcr);
  mcr &= ~TIOCM_RTS;
  mcr |= TIOCM_DTR;
  ioctl(file_descriptor_, TIOCMSET, &mcr);

  return true;
}

// Decode value from buffer into multimeter state.
void DecodeValueFromBuffer(const unsigned char *buffer,
                           DMMState *state) {
  bool read_dot = false;
  float value = 0.0f;
  float float_multiplier = 1.0f;

  // Initially there's no overflow.
  state->value_overflow = false;

  for (int i = 1; i < 8; i += 2) {
    unsigned int encoded = ((0x07 & buffer[i]) << 4) +
                            (0x0f & buffer[i + 1]);
    float digit;
    if (i != 1) {
      read_dot |= (0x08 & buffer[i]) >> 3;
    }
    switch (encoded) {
      case 0x7d: digit = 0; break;
      case 0x05: digit = 1; break;
      case 0x5b: digit = 2; break;
      case 0x1f: digit = 3; break;
      case 0x27: digit = 4; break;
      case 0x3e: digit = 5; break;
      case 0x7e: digit = 6; break;
      case 0x15: digit = 7; break;
      case 0x7f: digit = 8; break;
      case 0x3f: digit = 9; break;
      case 0x68:
        digit = 0;
        state->value_overflow = true;
        break;
      default:
        digit = 0;
    };
    if (read_dot) {
      float_multiplier *= 10.0f;
      value = static_cast<float>(digit) / float_multiplier + value;
    } else {
      value = static_cast<float>(digit) + value * 10.0f;
    }
  }
  if (buffer[1] & 0x08) {
    value = -value;
  }
  state->raw_value = value;
}

// Decode buffer into multimeter state.
static bool DecodeBuffer(const unsigned char *buffer,
                         DMMState *state) {
  DecodeValueFromBuffer(buffer, state);

  state->mode = UNKNOWN_MODE;
  state->multiplier = UNITY;

#define CHECK_AND_SET(frame_offset_, \
                      bit_, \
                      field_, \
                      initial_value_, \
                      value_) \
  { \
    if (buffer[frame_offset_] & (bit_)) { \
      assert(state->field_ == initial_value_); \
      state->field_ = value_; \
    } \
  } (void)0

#define CHECK_AND_SET_MULTIPLIER(frame_offset_, bit_, multiplier_) \
  CHECK_AND_SET(frame_offset_, bit_, multiplier, UNITY, multiplier_)

#define CHECK_AND_SET_MODE(frame_offset_, bit_, mode_) \
  CHECK_AND_SET(frame_offset_, bit_, mode, UNKNOWN_MODE, mode_)

  CHECK_AND_SET_MULTIPLIER(9,  0x02, KILO);
  CHECK_AND_SET_MULTIPLIER(9,  0x04, NANO);
  CHECK_AND_SET_MULTIPLIER(9,  0x08, MICRO);
  CHECK_AND_SET_MULTIPLIER(10, 0x02, MEGA);
  CHECK_AND_SET_MULTIPLIER(10, 0x08, MILI);

  CHECK_AND_SET_MODE(9,  0x01, DIODE);
  CHECK_AND_SET_MODE(11, 0x04, RESISTANCE);
  CHECK_AND_SET_MODE(11, 0x08, CAPACITY);
  CHECK_AND_SET_MODE(12, 0x02, FREQUENCE);
  if (state->mode != DIODE) {
    /* Special case for diode test: multimeter will display volts
     * in diode mode, and we need to distinguish volts mode from
     * diode mode still.
     */
    CHECK_AND_SET_MODE(12, 0x04, VOLTAGE);
  }
  CHECK_AND_SET_MODE(12, 0x08, AMPERS);
  CHECK_AND_SET_MODE(13, 0x01, TEMPERATURE);

#undef CHECK_AND_SET_NULTIPLIER
#undef CHECK_AND_SET_MODE
#undef CHECK_AND_SET

  state->beep = (buffer[10] & 0x01) != 0;
  state->hold = (buffer[11] & 0x01) != 0;
  state->relative = (buffer[11] & 0x02) != 0;
  state->low_battery = (buffer[12] & 0x01) != 0;

  /* Set current type. */
  if (buffer[0] & 0x08) {
    state->current_type = AC;
  } else {
    state->current_type = DC;
  }

  // Ensure all required data is decoded.
  if (state->mode == UNKNOWN_MODE) {
    return false;
  }

  return true;
}

bool UT60E::ReadMultimeter(DMMState *state) {
  unsigned char buffer[14];
  int length = 0;

#define SEQUENCE_ID(byte) (((byte) & 0xf0) >> 4)
  while (length != sizeof(buffer)) {
    unsigned char current_buffer[14];
    int current_length =
      ReadPort(current_buffer, sizeof(buffer) - length);

    if (current_length < 0) {
      /* ReadPort is supposed to always return something,
       * and the only case when it returns nicht is when
       * port was closed (i.e. when closing application).
       *
       * TODO(sergey): consider using some explicit flag
       *               which will indicate whether we
       *               need to break cycle here and exit.
       */
      return false;
    }

    if (length == 0) {
      // Ensure actual data buffer starts from sequence id 0x01
      for (int i = 0; i < current_length; i++) {
        if (SEQUENCE_ID(current_buffer[i]) == 1) {
          length = current_length - i;
          memcpy(buffer, current_buffer + i, length);
          break;
        }
      }
    } else {
      // Append currently read buffer to existing one.
      memcpy(buffer + length, current_buffer, current_length);
      length += current_length;
    }
  }

  // Ensure all the frames are here.
  for (unsigned int i = 0; i < sizeof(buffer); i++) {
    if (SEQUENCE_ID(buffer[i]) != i + 1) {
      return false;
    }
  }
#undef SEQUENCE_ID

  /* Now we've got all the frames in correct order,
   * so we could safely decode all of them.
   */
  return DecodeBuffer(buffer, state);
}
