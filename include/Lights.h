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
    void breath(uint16_t);
    void fire(uint16_t,uint16_t,uint16_t);
};
