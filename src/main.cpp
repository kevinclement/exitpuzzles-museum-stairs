#include <Arduino.h>
#include "consts.h"
#include "logic.h"

// Globals
Logic logic;

void status(int) {
  logic.status();
}

void incLevel(int) {
  logic.serial.print("changing level up...%s", CRLF);
  logic.incrementLevel();
}

void decLevel(int) {
  logic.serial.print("changing level down...%s", CRLF);
  logic.decrementLevel();
}

void resetLevel(int) {
  logic.serial.print("resetting to level 1...%s", CRLF);
  logic.resetLevel();
}

void solved(int) {
  logic.serial.print("forcing solved and drop...%s", CRLF);
  logic.solved();
}

void reboot(int) {
  ESP.restart();
}

void unsolvable(int) {
  logic.serial.print("toggling unsolvable...%s", CRLF);
  logic.unsolvable();
}

void sensorsDisabled(int) {
  logic.serial.print("toggling sensors...%s", CRLF);
  logic.sensorsDisabled();
}

void debug(int) {
  logic.serial.print("toggling sensor debug...%s", CRLF);
  logic.stairSensors.debug();
}

void setup() {
  logic.setup();
  logic.serial.print("Museum Stairs by kevinc...\n");

  logic.serial.registerCommand(SerialCommand("status",     's', &status,          "status",     "gets the status of device"));
  logic.serial.registerCommand(SerialCommand("drop",       'y', &solved,          "drop",       "forces solved state and opens device"));
  logic.serial.registerCommand(SerialCommand("up",         'u', &incLevel,        "up",         "moves the level up one"));
  logic.serial.registerCommand(SerialCommand("down",       'd', &decLevel,        "down",       "moves the level down one"));
  logic.serial.registerCommand(SerialCommand("fail",       'f', &resetLevel,      "fail",       "forces a fail and resets the level"));
  logic.serial.registerCommand(SerialCommand("unsolvable", 'n', &unsolvable,      "unsolvable", "toggle puzzle being unsolvable"));
  logic.serial.registerCommand(SerialCommand("sensors",    'e', &sensorsDisabled, "sensors",    "toggle sensors being disabled"));
  logic.serial.registerCommand(SerialCommand("debug",      'x', &debug,           "debug",      "debug sensors"));
  logic.serial.registerCommand(SerialCommand("reboot",     'r', &reboot,          "reboot",     "software reboot the device"));

  logic.serial.printHelp();
  logic.status();
}

void loop() {
  logic.handle();
  delay(10);
}

// PREVIOUS COMMANDS ---------------------------
// ---------------------------------------------
//  m => _logic.lights.toggleMagnet();
//       _logic.magnet.open();
//  v => _logic.audio.solved();
//  b => _logic.lights.toggleBowl();
//  a => _logic.audio.levelUp();
//  x => _logic.audio.stop();