#pragma once

#include "SerialManager.h"
#include "AudioPlayer.h"
#include "Magnet.h"
#include "Lights.h"
#include "StairSensors.h"

class Logic {
public:
  Logic();
  SerialManager serial;
  AudioPlayer audio;
  Magnet magnet;
  Lights lights;
  StairSensors stairSensors;

  int level = 1;

  void setup();
  void handle();
  void status();
  void incrementLevel();
  void changeLevel(int, bool);

private:
  void solved();
};

