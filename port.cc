// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)
//
// This is a file with implementation of platform-specific
// general purpose functions.

#include "port.h"

#include <cstdio>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

void SleepMilliseconds(int milliseconds) {
  usleep(milliseconds * 1000);
}

CommPortHandle OpenCommPort(const char *device) {
  CommPortHandle file_descriptor_;

  file_descriptor_ = open(device, O_RDONLY | O_NOCTTY | O_NONBLOCK);
  if (file_descriptor_ < 0) {
    perror(device);
    return COMM_PORT_INVALID;
  }

  /* Setup port in a way missing DCD wire from USB->RS232 adapters
   * does not lead to read() lock the execution and never read the
   * data.
   *
   * This is required for newer linux kernels, see the discussion:
   * https://bugs.launchpad.net/ubuntu/+source/linux/+bug/661321
   */
  struct termios term;
  if (tcgetattr(file_descriptor_, &term) != 0) {
    close(file_descriptor_);
    return COMM_PORT_INVALID;
  }
  cfmakeraw(&term);
  term.c_cflag = B2400 | CLOCAL | CS8 | CREAD;
  term.c_iflag = ICRNL;
  term.c_oflag = 0;
  term.c_lflag = ~ICANON;

  if (tcsetattr(file_descriptor_, TCSAFLUSH, &term) != 0) {
    close(file_descriptor_);
    return COMM_PORT_INVALID;
  }

  return file_descriptor_;
}

int ReadCommPort(CommPortHandle comm_port,
                 unsigned char *buffer,
                 int count) {
  return read(comm_port, buffer, count);
}

void CloseCommPort(CommPortHandle comm_port) {
  close(comm_port);
}
