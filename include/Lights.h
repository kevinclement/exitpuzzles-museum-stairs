#pragma once

#include "Arduino.h"

class Logic;

class Lights {
  public:
    Lights(Logic &logic);
    void setup();
    void handle();

    void moveToLevel(int);
    void changeMagnet(bool);
    void toggleMagnet();
    void changeBowl(bool);
    void toggleBowl();

  private:
    Logic &_logic;
    bool _debug;
};
