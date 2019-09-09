#include "Arduino.h"
#include "EEPROM.h"
#include "logic.h"
#include "consts.h"

#define STAIR_THRESH 100

bool _solved = false;

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
  audio.handle();

  if (_solved) {
    // TMP: allow reset by standing on top two steps
    if (stairSensors.bad_value > 400 && stairSensors.sensor_values[6] > STAIR_THRESH) {
      Serial.println("DEBUG: RESETTING");
      _solved = false;
    }
    return;
  }

  if (stairSensors.bad_value > 400 && level != 1) {
    Serial.printf("FAILED RESETTING - level is %d bad: %d\r\n", level, stairSensors.bad_value);
    changeLevel(1);
  }

  if (stairSensors.sensor_values[level - 1] > STAIR_THRESH) {
    Serial.printf("Passed level %d\r\n", level);
    changeLevel(level + 1);

    if (level == 8) { 
      solved();
    }
  }
}

void Logic::solved() {
  Serial.println("SOLVED!");
  _solved = true;
  magnet.open();
}

void Logic::incrementLevel() {
  changeLevel(level + 1);
}

void Logic::changeLevel(int newLevel) {
  level = newLevel;
  lights.moveToLevel(level);
  audio.levelUp();
}
