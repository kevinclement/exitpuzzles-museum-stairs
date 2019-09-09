#include "Arduino.h"
#include "EEPROM.h"
#include "logic.h"
#include "consts.h"

Logic::Logic() 
  : serial(*this),
    audio(*this),
    stepmotor(*this),
    notes(*this),
    stairSensors(*this)
{
}

void Logic::setup() {
  serial.setup();
  audio.setup();
  stairSensors.setup();
  notes.setup();
  stepmotor.setup();

  serial.printHelp();
}

void Logic::handle() {
  serial.handle();
  stairSensors.handle();

  // notes.handle();
  // stepmotor.handle();
  // audio.handle();

  delay(100);
}
