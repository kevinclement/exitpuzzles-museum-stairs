#pragma once

#include "Arduino.h"

class Logic;

class AudioPlayer {
  public:
    AudioPlayer(Logic &logic);
    void setup();
    void handle();

    byte volume_low = 0x2A;
    byte volume_high = 0x30;
    byte volume_whosh = 0x32;

    void idle();
    void levelUp();
    void failure();
    void stop();
    void solved();

  private:
    Logic &_logic;
    bool _solved = false;
};
