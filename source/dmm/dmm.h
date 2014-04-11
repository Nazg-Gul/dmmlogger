// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#ifndef DMM_H_
#define DMM_H_

#include <iostream>

// TODO(sergey): Think of using units instead of mode.
enum Mode {
  UNKNOWN_MODE,
  VOLTAGE,
  AMPERS,
  RESISTANCE,
  DIODE,
  CONTINUITY,
  FREQUENCE,
  TEMPERATURE,
  CAPACITY,
};

// Type of measuring current.
enum CurrentType {
  DC,
  AC,
};

// Multiplier applied on displayed value.
enum Multiplier {
  UNITY,
  NANO,
  MICRO,
  MILI,
  KILO,
  MEGA,
};

// State of the multimeter.
struct DMMState {
  DMMState()
    : mode(UNKNOWN_MODE),
      current_type(DC),
      multiplier(UNITY),
      raw_value(0.0f),
      value(0.0f),
      value_overflow(false),
      beep(false),
      low_battery(false),
      hold(false),
      relative(false) { }

  // Mode multiplimeter is currently measuring.
  Mode mode;

  // Type of current being measured (AC/DC).
  CurrentType current_type;

  // Multiplier being applyed on value displayed.
  Multiplier multiplier;

  // Value being displayed on the screen of multimeter.
  float raw_value;

  // Normalized to standard range value.
  float value;

  // Whether multimeter displays value overflow.
  bool value_overflow;

  // Whether beeper is active.
  bool beep;

  // Whether battery went low.
  bool low_battery;

  // Whether value is hold in the multimeter.
  bool hold;

  // Whether relative measurement is used.
  bool relative;
};

enum Result {
  RESULT_OK,
  RESULT_PROTOCOL_ERROR,
  RESULT_USER_ABORT,
};

// Basis mulstimeter descriptor.
class DMM {
 public:
  DMM(void);
  virtual ~DMM(void);

  // Connect to the multimeter.
  virtual bool Connect(void) = 0;

  // Disconnect from the multimeter.
  virtual void Disconnect(void) = 0;

  // Get current multimeter status.
  DMMState state(void);

  // Update multimeter state.
  Result Update(void);

  // Request all current operations (such as reading) to stop.
  void RequestStop(void);
 protected:
  // State of the multimeter.
  DMMState state_;

  // Boolean flag indicating whether all current activities shall be stopped.
  bool stop_requested_;

  // Read data from the multimeter and fill in new state.
  virtual Result ReadMultimeter(DMMState *state) = 0;
};

const char *UnitsFromMode(Mode mode);
const char *MultiplierStringify(Multiplier multiplier);
const char *CurrentTypeStringify(CurrentType current_type);

// A human-readable representation of the multimeter state.
std::ostream& operator <<(std::ostream &os,
                          const DMMState &state);

#endif
