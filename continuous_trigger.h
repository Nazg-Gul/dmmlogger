// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#ifndef CONTINUOUS_TRIGGER_H_
#define CONTINUOUS_TRIGGER_H_

#include "trigger.h"

// Trigger which sets on every update received from the multimeter.
class ContinuousTrigger : public Trigger {
 public:
  void Update(const DMMState &state);
};

#endif
