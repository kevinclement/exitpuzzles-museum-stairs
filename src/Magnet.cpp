#include "Arduino.h"
#include "Magnet.h"

#define MAGNET_PIN 16

bool high = true;
Magnet::Magnet(Logic &logic)
: _logic(logic)
{  
}

void Magnet::setup() {
  pinMode(MAGNET_PIN, OUTPUT);
}

void Magnet::open() {
  high = true;
}

void Magnet::handle() {
  digitalWrite(MAGNET_PIN, high ? HIGH : LOW);
}
