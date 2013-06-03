// DMMLogger - A6Q logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#include "dmm/dmm.h"

#include <cassert>

DMM::DMM(void)
  : stop_requested_(false) {
}

DMM::~DMM(void) {
}

DMMState DMM::state(void) {
  // TODO(sergey): add thread locking here.
  DMMState temp_state = state_;
  return temp_state;
}

bool DMM::Update(void) {
  /* Use current state as reference. This is so because multimeter protocol
   * could send incremental changes instead of sending the whole state.
   *
   * TODO(sergey): add thread locking here.
   */
  DMMState new_state;
  new_state = state_;

  bool result = ReadMultimeter(&new_state);

  if (result) {
    // Update current sttaus with new one.
    // TODO(sergey): add thread locking here.
    state_ = new_state;
    return true;
  }

  // Failed to load new state.
  return false;
}

void DMM::RequestStop(void) {
  stop_requested_ = true;
}

namespace {
const char *UnitsFromMode(Mode mode) {
  switch (mode) {
    case UNKNOWN_MODE:
      return "N/A";
    case VOLTAGE:
      return "V";
    case AMPERS:
      return "A";
    case RESISTANCE:
      return "Ohm";
    case DIODE:
      return "V";
    case CONTINUITY:
      return "Ohm";
    case FREQUENCE:
      return "Hz";
    case TEMPERATURE:
      return "C";
    case CAPACITY:
      return "F";
  }

  // Shall never happen.
  assert(!"Ukniwn mode");

  return "";
}

const char *MultiplierStringify(Multiplier multiplier) {
  switch (multiplier) {
    case UNITY:
      return "";
    case NANO:
      return "n";
    case MICRO:
      return "u";
    case MILI:
      return"m";
    case KILO:
      return "K";
    case MEGA:
      return "M";
  }

  // Shall never happen.
  assert(!"Unknown multiplier");

  return "";
}

const char *CurrentTypeStringify(CurrentType current_type) {
  if (current_type == AC) {
    return "AC";
  }

  assert(current_type == DC);

  return "DC";
}
}  // namespace

std::ostream& operator <<(std::ostream &os,
                          const DMMState &state) {
  if (state.value_overflow) {
    os << "0.L ";
  } else {
    os << state.raw_value << " ";
  }

  os << MultiplierStringify(state.multiplier);
  os << UnitsFromMode(state.mode);

  if (state.mode == VOLTAGE || state.mode == AMPERS) {
    os << " " << CurrentTypeStringify(state.current_type);
  }

  return os;
}
