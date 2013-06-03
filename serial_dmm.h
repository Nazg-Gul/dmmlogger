// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#ifndef SERIAL_DMM_H_
#define SERIAL_DMM_H_

#include "dmm.h"
#include "port.h"

#include <string>

// Basis descriptor for all serial-interfaced multimeters.
class SerialDMM : public DMM {
 public:
  explicit SerialDMM(const std::string &device);
  ~SerialDMM(void);

  bool Connect(void);
  void Disconnect(void);

 protected:
  // Name of the serial device to be connected to.
  std::string device_;

  // File descriptor of the device after connection.
  CommPortHandle file_descriptor_;

  // Read data from the port.
  int ReadPort(unsigned char *buffer, int count);
};

#endif
