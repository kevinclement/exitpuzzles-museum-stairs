#include "Arduino.h"
#include "EEPROM.h"
#include "logic.h"
#include "consts.h"

Logic::Logic() 
  : serial(*this),
    audio(*this),
    stepmotor(*this),
    lights(*this),
    stairSensors(*this)
{
}

void Logic::setup() {
  serial.setup();
  audio.setup();
  stairSensors.setup();
  lights.setup();
  stepmotor.setup();

  serial.printHelp();
}

void Logic::handle() {
  serial.handle();
  stairSensors.handle();
  lights.handle();
  
  // stepmotor.handle();
  // audio.handle();

  delay(100);
}
