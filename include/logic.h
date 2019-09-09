#pragma once

#include "SerialManager.h"
#include "AudioPlayer.h"
#include "Stepper.h"
#include "Notes.h"
#include "StairSensors.h"

class Logic {
public:
  Logic();
  SerialManager serial;
  AudioPlayer audio;
  Stepper stepmotor;
  Notes notes;
  StairSensors stairSensors;

  void setup();
  void handle();

private:
};

