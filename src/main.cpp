#include <Arduino.h>
#include "consts.h"
#include "logic.h"

// Globals
Logic logic;

void status(int) {
  logic.status();
}

void solved(int) {
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

  logic.serial.registerCommand(SerialCommand("status",  's', &status, "status", "gets the status of device"));
  logic.serial.registerCommand(SerialCommand("drop",    'y', &solved, "drop",   "forces solved state and opens device"));
  logic.serial.registerCommand(SerialCommand("debug",   'd', &debug,  "debug",  "debug sensors"));
  logic.serial.registerCommand(SerialCommand("reboot",  'r', &reboot, "reboot", "software reboot the device"));
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
  // else if (command == "l") {
  //   print("changing level...%s", CRLF);
  //   _logic.incrementLevel();
  // else if (command == "x") {
  //   print("resetting to level 1...%s", CRLF);
  //   _logic.lights.moveToLevel(1);
