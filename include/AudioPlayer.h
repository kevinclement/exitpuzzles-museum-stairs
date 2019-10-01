#pragma once

#include "Arduino.h"

class Logic;

class AudioPlayer {
  public:
    AudioPlayer(Logic &logic);
    void setup();
    void handle();

    byte volume_low = 0x16;
    byte volume_high = 0x1C;
    byte volume_whosh = 0x1E;

    void idle();
    void levelUp();
    void stop();
    void solved();

  private:
    Logic &_logic;
    bool _solved = false;
};
