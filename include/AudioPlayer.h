#pragma once

#include "Arduino.h"

class Logic;

class AudioPlayer {
  public:
    AudioPlayer(Logic &logic);
    void setup();
    void handle();

    void idle();
    void levelUp();
    void stop();
    void solved();

  private:
    Logic &_logic;

};
