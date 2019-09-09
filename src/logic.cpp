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

  for (int i=0; i < 7; i++) {
    Serial.printf("%d: %4d ", i+1, stairSensors.sensor_values[i]);
  }
  Serial.printf("BAD: %4d\r\n", stairSensors.bad_value);

  delay(100);
}
