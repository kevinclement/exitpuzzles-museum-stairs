#pragma once

#include "Arduino.h"

#define NUM_SENSORS 7

class Logic;

class StairSensors {
  public:
    StairSensors(Logic &logic);
    void setup();
    void handle();
    void debugSensors();

    int sensor_values[NUM_SENSORS] = { 0, 0, 0, 0, 0, 0, 0 };
    int bad_value = 0;

  private:
    Logic &_logic;
};
