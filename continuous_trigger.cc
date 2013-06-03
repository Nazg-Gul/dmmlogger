// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#include "continuous_trigger.h"

void ContinuousTrigger::Update(const DMMState &state) {
  (void) state;  // ignored.

  Set();
}
