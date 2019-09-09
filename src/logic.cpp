#include "Arduino.h"
#include "EEPROM.h"
#include "logic.h"
#include "consts.h"

#define STAIR_THRESH 100

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

  if (stairSensors.bad_value > STAIR_THRESH && level != 1) {
    Serial.println("FAILED RESETTING");
    changeLevel(1);
  }

  if (level == 1 && stairSensors.sensor_values[0] > STAIR_THRESH) {
    Serial.println("Passed level 1");
    changeLevel(2);
  }

  delay(100);
}

void Logic::incrementLevel() {
  changeLevel(level + 1);
}

void Logic::changeLevel(int newLevel) {
  level = newLevel;
  lights.moveToLevel(level);
}
