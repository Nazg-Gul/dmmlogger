// DMMLogger - A logging software for digital multimeters
// Copyright 2014 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#ifndef TRIGGER_FACTORY_H_
#define TRIGGER_FACTORY_H_

#include <string>
#include <vector>

#include "dmm/trigger.h"

class TriggerFactory {
 public:
  enum TriggerType {
    TRIGGER_TYPE_CONTINUOUS,
    TRIGGER_TYPE_SETTLE,
  };

  struct TriggerInfo {
    TriggerType type;
    std::string name;
  };

  TriggerFactory(void);

  const std::vector<TriggerInfo> &GetTriggers(void) {
    return triggers_;
  }

  Trigger *CreateTrigger(TriggerType type);

 private:
  void RegisterTrigger(TriggerType type, std::string name);

  std::vector<TriggerInfo> triggers_;
};

#endif  // TRIGGER_FACTORY_H_
