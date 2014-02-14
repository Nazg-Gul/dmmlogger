// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#ifndef UI_READER_THREAD_H_
#define UI_READER_THREAD_H_

#include <QThread>

#include "dmm/dmm.h"
#include "dmm/trigger.h"

class ReaderThread : public QThread {
 public:
  explicit ReaderThread(DMM *dmm, Trigger *trigger);

  void run(void);

 private:
  // Digital multimeter we're reading from.
  DMM *dmm_;

  // Trigger used for logging
  Trigger *trigger_;
};

#endif
