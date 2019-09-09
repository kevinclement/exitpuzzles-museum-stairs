#include "Arduino.h"
#include "EEPROM.h"
#include "logic.h"
#include "consts.h"

Logic::Logic() 
  : serial(*this),
    audio(*this),
    magnet(*this),
    lights(*this),
    stairSensors(*this)
{
}

void Logic::setup() {
  serial.setup();
  audio.setup();
  stairSensors.setup();
  lights.setup();
  magnet.setup();

  serial.printHelp();
}

void Logic::handle() {
  serial.handle();
  stairSensors.handle();
  lights.handle();
  magnet.handle();
  // audio.handle();

  delay(100);
}
