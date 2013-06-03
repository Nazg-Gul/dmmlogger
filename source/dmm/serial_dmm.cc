// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#include "dmm/serial_dmm.h"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>

SerialDMM::SerialDMM(const std::string &device)
  : device_(device),
    file_descriptor_(COMM_PORT_INVALID) {
}

SerialDMM::~SerialDMM(void) {
  if (file_descriptor_ != COMM_PORT_INVALID) {
    // Disconnect if still connected.
    Disconnect();
  }
}

bool SerialDMM::Connect(void) {
  assert(file_descriptor_ == COMM_PORT_INVALID);

  file_descriptor_ = OpenCommPort(device_.c_str());
  if (file_descriptor_ == COMM_PORT_INVALID) {
    return false;
  }
  return true;
}

void SerialDMM::Disconnect(void) {
  assert(file_descriptor_ != COMM_PORT_INVALID);

  CloseCommPort(file_descriptor_);
  file_descriptor_ = COMM_PORT_INVALID;
}

int SerialDMM::ReadPort(unsigned char *buffer, int count) {
  int length;

  /* Port is non-blocking, so we're using busy wait,
   * which means we'll try to read something from port
   * for until something was read,
   *
   * If nothing was read, we'll wait a bit and then
   * try reading again.
   *
   * Alternative would be to use poll(), but for now
   * it's not clear why it'll be better from breaking
   * reading (i.e. when application exit) pov.
   */
  do {
    if (file_descriptor_ == COMM_PORT_INVALID) {
      // Multimeter was disconnected while reading,
      // exit read cycle with no buffer loaded.
      return -1;
    }

    if (stop_requested_) {
      // All reading was requested to stop from another thread.
      return -1;
    }

    length = ReadCommPort(file_descriptor_, buffer, count);

    // If something was read return instantly,
    // otherwise wait a bit before next read attempt.
    if (length < 0) {
      SleepMilliseconds(10);
    }
  } while (length < 0);
  return length;
}
