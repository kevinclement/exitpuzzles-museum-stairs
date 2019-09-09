#pragma once

#include "Arduino.h"

class Logic;

class Lights {
  public:
    Lights(Logic &logic);
    void setup();
    void handle();

  private:
    Logic &_logic;
};
