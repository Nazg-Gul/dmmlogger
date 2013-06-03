// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)
//
// This is a file with declaration of platform-specific
// general purpose functions.

#ifndef PORT_H_
#define PORT_H_

// Make a delay of specified milliseconds.
void SleepMilliseconds(int milliseconds);

// Value of invalid (unconnected) serial port descriptor.
#if defined(__linux__)
enum { COMM_PORT_INVALID = -1 };
typedef int CommPortHandle;
#elif defined(_WIN32)
# include <windows.h>
# define COMM_PORT_INVALID INVALID_HANDLE_VALUE
typedef HANDLE CommPortHandle;
#endif

// Open COM port device for reading.
// Port is opening in non-blocking mode.
// In case of error COMM_PORT_INVALID returns.
CommPortHandle OpenCommPort(const char *device);

// Read data from specified COM port into buffer.
// Max of count bytes are read.
// Returns number of read bytes.
int ReadCommPort(CommPortHandle comm_port,
                 unsigned char *buffer,
                 int count);

// Set control lines, needed in some special cases.
bool CommPortSetControl(CommPortHandle comm_port,
                        bool control_dtr,
                        bool control_rts);

// Close COM port handle.
void CloseCommPort(CommPortHandle comm_port);

#endif
