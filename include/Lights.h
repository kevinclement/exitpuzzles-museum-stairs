#pragma once

#include "Arduino.h"

class Logic;

class Lights {
  public:
    Lights(Logic &logic);
    void setup();
    void handle();

    bool magnetLightOn = false;
    bool bowlOn = false;

    void moveToLevel(int);
    void changeMagnet(bool);
    void toggleMagnet();
    void changeBowl(bool);
    void toggleBowl();

    

  private:
    Logic &_logic;
    bool _debug;
    uint16_t _level = 0;
    bool _fade = false;
    void breath(uint16_t);
    void fire(uint16_t,uint16_t,uint16_t);
    void bowl();
};
