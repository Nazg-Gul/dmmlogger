// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#ifndef UT60E_H_
#define UT60E_H_

#include "dmm/serial_dmm.h"

// Uni-T UT60E multimeter descriptor.
class UT60E : public SerialDMM {
 public:
  explicit UT60E(const std::string &device);

  bool Connect(void);

  protected:
    // Read the multimeter to the new state.
    bool ReadMultimeter(DMMState *state);
};

#endif
