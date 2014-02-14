// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#include "ui/reader_thread.h"
#include "dmm/port.h"

ReaderThread::ReaderThread(DMM *dmm, Trigger *trigger)
    : dmm_(dmm),
      trigger_(trigger) {
}

void ReaderThread::run(void) {
  while (true) {
    Result result = dmm_->Update();
    if (result == RESULT_OK) {
      trigger_->Update(dmm_->state());
    } else if (result == RESULT_USER_ABORT) {
      break;
    }
    SleepMilliseconds(10);
  }
}
