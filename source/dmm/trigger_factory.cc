// DMMLogger - A logging software for digital multimeters
// Copyright 2014 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#include <cassert>

#include "dmm/trigger_factory.h"

#include "dmm/continuous_trigger.h"
#include "dmm/settle_trigger.h"

TriggerFactory::TriggerFactory(void) {
  RegisterTrigger(TRIGGER_TYPE_CONTINUOUS, "Continuous");
  RegisterTrigger(TRIGGER_TYPE_SETTLE, "Settle");
}

void TriggerFactory::RegisterTrigger(TriggerType type, std::string name) {
  TriggerInfo info;
  info.type = type;
  info.name = name;
  triggers_.push_back(info);
}

Trigger *TriggerFactory::CreateTrigger(TriggerType type) {
  Trigger *trigger = NULL;
  switch (type) {
    case TRIGGER_TYPE_CONTINUOUS:
      trigger = new ContinuousTrigger();
      break;
    case TRIGGER_TYPE_SETTLE:
      trigger = new SettleTrigger();
      break;
    default:
      assert(!"Unknown trigger type passed to the factory.");
  }
  return trigger;
}
