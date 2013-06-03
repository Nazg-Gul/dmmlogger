// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#ifndef SETTLE_TRIGGER_H_
#define SETTLE_TRIGGER_H_

#include "dmm/trigger.h"

// Trigger which sets when displaying value settled.
class SettleTrigger : public Trigger {
 public:
  SettleTrigger(void);
  void Update(const DMMState &state);
 private:
  enum Action {
    WAIT_FOR_NO_DATA,
    WAIT_FOR_STABLE_DATA,
  };

  DMMState previous_state_;
  Action action_;
  int count_;
};

#endif
