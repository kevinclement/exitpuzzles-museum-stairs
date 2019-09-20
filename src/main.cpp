#include <Arduino.h>
#include "logic.h"

// Globals
Logic logic;

void status(int v) {
  logic.status();
}

void solved(int v) {
  logic.solved();
}

void reboot(int v) {
  ESP.restart();
}

void setup() {
  logic.setup();

  logic.serial.registerCommand(SerialCommand("status",  's', &status, "status", "gets the status of device"));
  logic.serial.registerCommand(SerialCommand("drop",    'y', &solved, "drop",   "forces solved state and opens device"));
  logic.serial.registerCommand(SerialCommand("reboot",  'r', &reboot, "reboot", "software reboot the device"));
}

void loop() {
  logic.handle();
  delay(10);
}