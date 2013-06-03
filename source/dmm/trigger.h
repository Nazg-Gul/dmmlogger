// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#ifndef TRIGGER_H_
#define TRIGGER_H_

#include "dmm/dmm.h"

class Trigger {
 public:
  Trigger(void);

  virtual ~Trigger(void);

  // Update trigger state using new multimeter state.
  virtual void Update(const DMMState &state) = 0;

  // Reset tiggered state, calls when value was handled
  // after trigger happened.
  void Reset(void);

  // Get troggered state.
  bool triggered(void);

 protected:
  // Set triggered state.
  void Set(void);

 private:
  bool triggered_;
};

#endif
