// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#include <cstdlib>
#include <cmath>
#include <cstdio>

#include "port.h"
#include "ut60e.h"
#include "settle_trigger.h"
#include "continuous_trigger.h"

int main(int argc, char **argv) {
  UT60E dmm("/dev/ttyUSB0");
  ContinuousTrigger trigger;

  if (dmm.Connect()) {
    while (true) {
      if (dmm.Update()) {
        DMMState state = dmm.state();
        trigger.Update(state);
        if (trigger.triggered()) {
          std::cout << state << std::endl;
          trigger.Reset();
        }
      }
      SleepMilliseconds(10);
    }
    dmm.Disconnect();
  }
  return EXIT_SUCCESS;
}
