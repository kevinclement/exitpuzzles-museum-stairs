#include <Arduino.h>
#include "consts.h"
#include "logic.h"

// Globals
Logic logic;

void status(int) {
  logic.status();
}

void incLevel(int) {
  logic.serial.print("changing level...%s", CRLF);
  logic.incrementLevel();
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

void debug(int) {
  logic.serial.print("toggling sensor debug...%s", CRLF);
  logic.stairSensors.debug();
}

void setup() {
  logic.setup();

  logic.serial.registerCommand(SerialCommand("status",  's', &status,     "status", "gets the status of device"));
  logic.serial.registerCommand(SerialCommand("drop",    'y', &solved,     "drop",   "forces solved state and opens device"));
  logic.serial.registerCommand(SerialCommand("level",   'l', &incLevel,   "level",  "moves the level up one"));
  logic.serial.registerCommand(SerialCommand("fail",    'f', &resetLevel, "fail",   "forces a fail and resets the level"));
  logic.serial.registerCommand(SerialCommand("debug",   'd', &debug,      "debug",  "debug sensors"));
  logic.serial.registerCommand(SerialCommand("reboot",  'r', &reboot,     "reboot", "software reboot the device"));
}

void loop() {
  logic.handle();
  delay(10);
}

// PREVIOUS COMMANDS ---------------------------
// ---------------------------------------------

  // else if (command == "m") {
  //   print("toggling magnet...%s", CRLF);
  //   _logic.lights.toggleMagnet();
  //   _logic.magnet.open();
  // else if (command == "v") {
  //   print("adjusting volume..%s", CRLF);
  //   _logic.audio.solved();
  // else if (command == "b") {
  //   print("toggling bowl...%s", CRLF);
  //   _logic.lights.toggleBowl();
  // else if (command == "a") {
  //   print("playing audio...%s", CRLF);
  //   _logic.audio.levelUp();
  // else if (command == "audiostop") {
  //   print("stoping audio...%s", CRLF);
  //   _logic.audio.stop();
