#include "Arduino.h"
#include "StairSensors.h"
#include "logic.h"

int sensor_pins[NUM_SENSORS] = { A11, A5, A4, A3, A2, A1, A0 };
int bad_pin = A10;

StairSensors::StairSensors(Logic &logic)
: _logic(logic)
{
}

void StairSensors::setup() {
}

void StairSensors::debug() {
  _debug = !_debug;
}

void StairSensors::printSensors() {
  for (int i=0; i < 7; i++) {
    _logic.serial.print("%d: %4d ", i+1, sensor_values[i]);
  }
  _logic.serial.print("BAD: %4d\r\n", bad_value);
}

void StairSensors::handle() {
  bad_value = analogRead(bad_pin);
  for (int i=0; i < NUM_SENSORS; i++) {
    sensor_values[i] = analogRead(sensor_pins[i]);
  }

  if (_debug) {
    printSensors();
  }
}
