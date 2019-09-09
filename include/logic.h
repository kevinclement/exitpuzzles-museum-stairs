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
  void incrementLevel();
  void changeLevel(int);

private:
};

