// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#include "dmm/settle_trigger.h"

#include <cmath>

SettleTrigger::SettleTrigger(void)
    : action_(WAIT_FOR_NO_DATA) {
}

void SettleTrigger::Update(const DMMState &state) {
  if (action_ == WAIT_FOR_NO_DATA) {
    // TODO(sergey): Only works for resistance and diode modes.
    if (state.value_overflow) {
      action_ = WAIT_FOR_STABLE_DATA;
      previous_state_ = state;
      count_ = 0;
    }
  } else if (action_ == WAIT_FOR_STABLE_DATA) {
    if (state.mode != previous_state_.mode) {
      // Measuring mode was changed, wait for leads to
      // be detached and attached again for more accurate
      // triggering.
      action_ = WAIT_FOR_NO_DATA;
    } else if (!state.value_overflow) {
      if (!previous_state_.value_overflow) {
        // Currently used stupid absolute delta, better make it
        // so leats meaningful digit is allowed to jump a bit.
        if (previous_state_.multiplier == state.multiplier &&
            fabs(previous_state_.raw_value - state.raw_value) < 1e-4) {
          // For now wait for the same value displayed within 3 iterations,
          // but better to replace this with timer.
          count_++;
          if (count_ == 3) {
            Set();
            action_ = WAIT_FOR_NO_DATA;
          }
        }
      }
      previous_state_ = state;
    }
  }
}
