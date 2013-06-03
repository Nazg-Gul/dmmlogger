// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)
//
// This is a file with implementation of platform-specific
// general purpose functions.

#include "dmm/port.h"

#if defined(__linux__)
#include <cstdio>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

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
   *
   * Some additional tweaks are made to port settings to make
   * UT-60E workign fine. Other multimeters might require other
   * settings, and in that case we'll just move such tweaks to
   * multimeter-specific connection function.
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

bool CommPortSetControl(CommPortHandle comm_port,
                        bool control_dtr,
                        bool control_rts) {
  unsigned int mcr = 0;
  ioctl(comm_port, TIOCMGET, &mcr);
  if (control_dtr) {
    mcr |= TIOCM_DTR;
  } else {
    mcr &= ~TIOCM_DTR;
  }
  if (control_rts) {
    mcr |= ~TIOCM_RTS;
  } else {
    mcr &= ~TIOCM_RTS;
  }
  ioctl(comm_port, TIOCMSET, &mcr);
  // TODO(sergey): Proper error handling.
  return true;
}

void CloseCommPort(CommPortHandle comm_port) {
  close(comm_port);
}
#elif defined(_WIN32)
void SleepMilliseconds(int milliseconds) {
  Sleep(milliseconds);
}

CommPortHandle OpenCommPort(const char *device) {
  CommPortHandle file_descriptor;
  file_descriptor = CreateFile(device,
                               GENERIC_READ,
                               0,
                               NULL,
                               OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL,
                               0);

  if (file_descriptor == COMM_PORT_INVALID) {
    return COMM_PORT_INVALID;
  }

  // Default COM port settings.
  // Tested with UT60E only, could be so different
  // multimeter wil lrequire different settings.
  DCB control = {0};
  control.DCBlength = sizeof(DCB);
  if (!GetCommState(file_descriptor, &control )) {
    CloseCommPort(file_descriptor);
    return COMM_PORT_INVALID;
  }

  control.ByteSize = DATABITS_8;
  control.BaudRate = 2400;
  control.Parity = 0;

  if (!SetCommState(file_descriptor, &control)) {
    CloseCommPort(file_descriptor);
    return COMM_PORT_INVALID;
  }
  return file_descriptor;
}

int ReadCommPort(CommPortHandle comm_port,
                 unsigned char *buffer,
                 int count) {
  DWORD bytes_read;
  if (ReadFile(comm_port, buffer, count, &bytes_read, NULL)) {
    return bytes_read;
  }
  return -1;
}

bool CommPortSetControl(CommPortHandle comm_port,
                        bool control_dtr,
                        bool control_rts) {
  DCB control = {0};
  control.DCBlength = sizeof(DCB);

  if (!GetCommState(comm_port, &control )) {
    return false;
  }

  control.fDtrControl = control_dtr ? 1 : 0;
  control.fRtsControl = control_rts ? 1 : 0;

  if (!SetCommState(comm_port, &control)) {
    return false;
  }
  return true;
}

void CloseCommPort(CommPortHandle comm_port) {
   CloseHandle(comm_port);
}
#endif
