#pragma once

#include "SerialManager.h"
#include "AudioPlayer.h"
#include "Stepper.h"
#include "Lights.h"
#include "StairSensors.h"

class Logic {
public:
  Logic();
  SerialManager serial;
  AudioPlayer audio;
  Stepper stepmotor;
  Lights lights;
  StairSensors stairSensors;

  void setup();
  void handle();

private:
};

