// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#include "dmm/dummy_dmm.h"

DummyDMM::DummyDMM(void) : is_first_reading_(true) {
}

bool DummyDMM::Connect(void) {
  return true;
}

void DummyDMM::Disconnect(void) {
}

Result DummyDMM::ReadMultimeter(DMMState *state) {
  for (int i = 0; i < 10; i++) {
    if (this->stop_requested_) {
      return RESULT_USER_ABORT;
    }
    SleepMilliseconds(20);
  }
  state->mode = VOLTAGE;
  state->current_type = DC;
  state->multiplier = UNITY;
  state->raw_value = 30.0f;
  state->value = 0.0f;
  state->value_overflow = is_first_reading_;
  state->beep = false;
  state->low_battery = false;
  state->hold = false;
  state->relative = false;
  is_first_reading_ = false;
  return RESULT_OK;
}
