#pragma once

#include "Arduino.h"

class Logic;

class Magnet {
  public:
    Magnet(Logic &logic);
    void setup();
    void handle();

    void open();

  private:
    Logic &_logic;
    
};
