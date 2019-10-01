#pragma once

#include "Arduino.h"

class Logic;

class Magnet {
  public:
    Magnet(Logic &logic);
    void setup();
    void handle();

    bool high = true;
    void open();

  private:
    Logic &_logic;
    
};
