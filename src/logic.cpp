#include "Arduino.h"
#include "EEPROM.h"
#include "logic.h"
#include "consts.h"
#include "version.h"

#define STAIR_GOOD_THRESH 100
#define STAIR_BAD_THRESH 400
#define MAGNET_SOLVED_WAIT_TIME 2800

unsigned long _solved_at = 0;
bool _unsolvable = false;
bool _sensors_disabled = false;

Logic::Logic() 
  : serial(),
    audio(*this),
    magnet(*this),
    lights(*this),
    stairSensors(*this)
{
}

void Logic::setup() {
  // empty name disables bluetooth otherwise sensors wont work
  serial.setup("");

  audio.setup();
  stairSensors.setup();
  lights.setup();
  magnet.setup();
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
      serial.print("logic: solved wait time elapsed.  dropping magnet.\r\n");
      magnet.open();
      lights.changeMagnet(true);
      _solvedRan = true;

      status();
    }

    return;
  }

  bool inc = false;
  bool reset = false;

  // only react to sensors if they aren't disabled
  if (!_sensors_disabled) {
    if (stairSensors.bad_value > STAIR_BAD_THRESH && level != 1) {
      reset = true;
    }

    if (stairSensors.sensor_values[level - 1] > STAIR_GOOD_THRESH) {
      serial.print("sensor thresh l:%d v:%d\r\n", level, stairSensors.sensor_values[level - 1]);
      inc = true;
    }
  }

  // if its unsolvable, than randomize increments above level 1
  if (_unsolvable && inc && level > 1) {
    inc = random(2) == 0;
    reset = !inc;
  }

  if (inc) {
    serial.print("Passed level %d\r\n", level);
    incrementLevel();

    if (_unsolvable && level == 7) {
      resetLevel();
    } else if (!_unsolvable && level == 8) {
      solved();
    }
  } else if (reset) {
    serial.print("FAILED RESETTING - level is %d bad: %d\r\n", level, stairSensors.bad_value);
    resetLevel();
  }
}

void Logic::solved() {
  serial.print("SOLVED!\r\n");

  // change level to 8, in case this is a forced solved, otherwise its a noop
  changeLevel(8, false);

  _solved_at = millis();
  audio.solved();
}

void Logic::incrementLevel() {
  changeLevel(level + 1, false);
}

void Logic::decrementLevel() {
  changeLevel(level - 1, false);
}

void Logic::resetLevel() {
  changeLevel(1, true);
}

void Logic::unsolvable() {
  _unsolvable = !_unsolvable;
  status();
}

void Logic::sensorsDisabled() {
  _sensors_disabled = !_sensors_disabled;
  status();
}

void Logic::changeLevel(int newLevel, bool failure) {
  level = newLevel;
  lights.moveToLevel(level);

  if (!failure) {
    audio.levelUp();
  } else {
    // audio.stop();
    audio.failure();
  }

  status();
}

void Logic::status() {
  char cMsg[254];
  sprintf(cMsg, 
    "status="
      "version:%s,"
      "gitDate:%s,"
      "buildDate:%s,"
      
      "level:%d,"
      "solved:%s,"
      "bowl:%s,"
      "magnet:%s,"
      "magnetLight:%s,"
      "volumeLow:%d,"
      "volumeHigh:%d,"
      "volumeWhosh:%d,"
      "unsolvable:%s,"
      "sensorsDisabled:%s,"

      "%s"
    , GIT_HASH,
      GIT_DATE,
      DATE_NOW,

      level,
      _solved_at > 0 ? "true" : "false",
      lights.bowlOn ? "true" : "false",
      magnet.high ? "true" : "false",
      lights.magnetLightOn ? "true" : "false",
      audio.volume_low,
      audio.volume_high,
      audio.volume_whosh,
      _unsolvable ? "true" : "false",
      _sensors_disabled ? "true" : "false",

      CRLF);

  serial.print(cMsg);
}
