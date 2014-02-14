// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <cstring>

#include "dmm/ut60e.h"
#include "dmm/settle_trigger.h"
#include "dmm/continuous_trigger.h"

#if defined(__linux__)
#  include <signal.h>
#endif

namespace {
bool stop_requetsed = false;
DMM *current_dmm = NULL;

void PrintUsage(const char *argv0) {
  fprintf(stderr,
          "Usage: %s --device <serial_device_name> [--trigger <trigger>]\n",
          argv0);
  fprintf(stderr, "Supported triggers: continous, settle\n");
  fprintf(stderr, "\nNOTE: Currently only UT60E multimeter is supported.\n");
}

void PrintUsageAndExit(const char *argv0) {
  PrintUsage(argv0);
  exit(EXIT_FAILURE);
}

Trigger *CreateTriggerFromName(const char *trigger_name) {
  if (strcmp(trigger_name, "continuous") == 0) {
    return new ContinuousTrigger();
  } else if (strcmp(trigger_name, "settle") == 0) {
    return new SettleTrigger();
  }
  return NULL;
}

void RequestStop(void) {
  fprintf(stderr, "Exiting...\n");
  current_dmm->RequestStop();
  stop_requetsed = true;
}

#if defined(__linux__)
void sighandler(int signal) {
  RequestStop();
}

void InstallCtrlCHandler(void) {
  signal(SIGABRT, &sighandler);
  signal(SIGTERM, &sighandler);
  signal(SIGINT, &sighandler);
}
#elif defined(_WIN32)
BOOL WINAPI ConsoleHandler(DWORD event) {
  if (event == CTRL_C_EVENT) {
    RequestStop();
  }
  return TRUE;
}

void InstallCtrlCHandler(void) {
  SetConsoleCtrlHandler(ConsoleHandler, TRUE);
}
#endif
}  // namespace

int main(int argc, char **argv) {
  const char *device_name = NULL;
  const char *trigger_name = NULL;

  /* Parse command line arguments.
   *
   * This is a bit stupid to cycle via argv manually,
   * but this is better than being dependend of just
   * anither 3d party library just to get two options.
   */
#define EXPECT_NEXT_ARGUMENT() \
  { \
    if (argument_index + 1 >= argc) { \
      PrintUsageAndExit(argv[0]); \
    } \
  } (void)0
  int argument_index = 1;
  while (argument_index < argc) {
    if (strcmp(argv[argument_index], "--device") == 0) {
      if (device_name != NULL) {
        PrintUsageAndExit(argv[0]);
      }
      EXPECT_NEXT_ARGUMENT();
      argument_index++;
      device_name = argv[argument_index];
    } else if (strcmp(argv[argument_index], "--trigger") == 0) {
      if (trigger_name != NULL) {
        PrintUsageAndExit(argv[0]);
      }
      EXPECT_NEXT_ARGUMENT();
      argument_index++;
      trigger_name = argv[argument_index];
    }
    argument_index++;
  }
#undef EXPECT_NEXT_ARGUMENT

  // Check whether all options are there and correct.
  if (device_name == NULL) {
    fprintf(stderr, "No device specified,");
    PrintUsageAndExit(argv[0]);
  }

  if (trigger_name == NULL) {
    trigger_name = "continuous";
  }

  if (strcmp(trigger_name, "continuous") != 0 &&
      strcmp(trigger_name, "settle") != 0) {
    fprintf(stderr, "Wrong value provided for trigger.");
    PrintUsageAndExit(argv[0]);
  }

  UT60E dmm(device_name);
  current_dmm = &dmm;

  // Install Ctrl-C handler to exit.
  InstallCtrlCHandler();

  Trigger *trigger = CreateTriggerFromName(trigger_name);

  if (dmm.Connect()) {
    // Main read cycle, for until stop was requested
    // update the status of multimeter and print updates
    // to the console.
    while (stop_requetsed == false) {
      if (dmm.Update()) {
        DMMState state = dmm.state();
        trigger->Update(state);
        if (trigger->triggered()) {
          std::cout << state << std::endl;
          trigger->Reset();
        }
      }
      SleepMilliseconds(10);
    }
    dmm.Disconnect();
  }

  delete trigger;

  return EXIT_SUCCESS;
}
