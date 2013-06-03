// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#include "trigger.h"

Trigger::Trigger(void)
  : triggered_(false) {
}

Trigger::~Trigger(void) {
}

bool Trigger::triggered(void) {
  return triggered_;
}

void Trigger::Reset(void) {
  triggered_ = false;
}

void Trigger::Set(void) {
  triggered_ = true;
}
