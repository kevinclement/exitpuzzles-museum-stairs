#pragma once

#include "Arduino.h"

class Logic;

class StairSensors {
  public:
    StairSensors(Logic &logic);
    void setup();
    void handle();

  private:
    Logic &_logic;
};
