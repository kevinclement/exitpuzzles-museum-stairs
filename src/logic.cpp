#include "Arduino.h"
#include "EEPROM.h"
#include "logic.h"
#include "consts.h"

#define STAIR_GOOD_THRESH 100
#define STAIR_BAD_THRESH 400

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
    return;
  }

  if (stairSensors.bad_value > STAIR_BAD_THRESH && level != 1) {
    Serial.printf("FAILED RESETTING - level is %d bad: %d\r\n", level, stairSensors.bad_value);
    changeLevel(1);
  }

  if (stairSensors.sensor_values[level - 1] > STAIR_GOOD_THRESH) {
    Serial.printf("Passed level %d\r\n", level);
    incrementLevel();

    if (level == 8) { 
      solved();
    }
  }
}

void Logic::solved() {
  Serial.println("SOLVED!");
  _solved = true;
  magnet.open();
  audio.solved();
}

void Logic::incrementLevel() {
  changeLevel(level + 1);
}

void Logic::changeLevel(int newLevel) {
  level = newLevel;
  lights.moveToLevel(level);
  audio.levelUp();
}
