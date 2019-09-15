#include "Arduino.h"
#include "EEPROM.h"
#include "logic.h"
#include "consts.h"

#define STAIR_GOOD_THRESH 100
#define STAIR_BAD_THRESH 400
#define MAGNET_SOLVED_WAIT_TIME 2800

unsigned long _solved_at = 0;

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
  status();
}

void Logic::handle() {
  serial.handle();
  stairSensors.handle();
  lights.handle();
  magnet.handle();
  audio.handle();

  if (_solved_at > 0) {
    static bool _solvedRan = false;

    if (!_solvedRan && millis() - _solved_at > MAGNET_SOLVED_WAIT_TIME) {
      Serial.println("logic: solved wait time elapsed.  dropping magnet.");
      magnet.open();
      lights.changeMagnet(true);
      _solvedRan = true;

      status();
    }

    return;
  }

  if (stairSensors.bad_value > STAIR_BAD_THRESH && level != 1) {
    Serial.printf("FAILED RESETTING - level is %d bad: %d\r\n", level, stairSensors.bad_value);
    changeLevel(1, true);

    status();
  }

  if (stairSensors.sensor_values[level - 1] > STAIR_GOOD_THRESH) {
    Serial.printf("Passed level %d\r\n", level);
    incrementLevel();

    if (level == 8) {
      solved();
    }

    status();
  }
}

void Logic::solved() {
  Serial.println("SOLVED!");
  _solved_at = millis();
  audio.solved();
}

void Logic::incrementLevel() {
  changeLevel(level + 1, false);
}

void Logic::changeLevel(int newLevel, bool failure) {
  level = newLevel;
  lights.moveToLevel(level);

  if (!failure) {
    audio.levelUp();
  } else {
    audio.stop();
  }
}

void Logic::status() {
  serial.print(
    "status=level:%d,solved:%s,bowl:%s,magnet:%s,"
    "magnetLight:%s,volumeLow:%d,volumeHigh:%d,volumeWhosh:%d%s",

    level,
    _solved_at > 0 ? "true" : "false",
    lights.bowlOn ? "true" : "false",
    magnet.high ? "true" : "false",
    lights.magnetLightOn ? "true" : "false",
    audio.volume_low,
    audio.volume_high,
    audio.volume_whosh,
    CRLF);
}
