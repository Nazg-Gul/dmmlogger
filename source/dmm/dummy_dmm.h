// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#ifndef DUMMY_DMM_H_
#define DUMMY_DMM_H_

#include "dmm/dmm.h"
#include "dmm/port.h"

#include <string>

class DummyDMM : public DMM {
 public:
  DummyDMM(void);
  bool Connect(void);
  void Disconnect(void);

 private:
  Result ReadMultimeter(DMMState *state);
  bool is_first_reading_;
};

#endif
