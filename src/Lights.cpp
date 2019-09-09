#include "Arduino.h"
#include "Lights.h"
#include <FastLED.h>

#define CHIPSET       WS2811
#define NUM_LEDS      75
#define NUM_LEDS_LEFT 170 // TODO: could add 5 more
#define NUM_LEDS_MAG  13

#define BRIGHTNESS  80

CRGB left[NUM_LEDS_LEFT];
CRGB middle[NUM_LEDS];
CRGB right[NUM_LEDS];

CRGB magnet[NUM_LEDS_MAG];

bool magnetLightOn = false;

Lights::Lights(Logic &logic)
: _logic(logic)
{  
}

void Lights::moveToLevel(int level) {

}

void Lights::toggleMagnet() {
    changeMagnet(!magnetLightOn);
}

void Lights::changeMagnet(bool show) {
  magnetLightOn = show;
  for( int j = 0; j < NUM_LEDS_MAG; j++) {
    magnet[j] = show ? CRGB::White : CRGB::Black;
  }
}

void Lights::setup() {
  // 22 = left
  // 23 = middle
  // 21 = right
  FastLED.addLeds<CHIPSET, 22, GRB>(left, NUM_LEDS_LEFT).setCorrection( TypicalLEDStrip );;
  FastLED.addLeds<CHIPSET, 23, GRB>(middle, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, 21, GRB>(right, NUM_LEDS).setCorrection( TypicalLEDStrip );

  FastLED.addLeds<CHIPSET, 17, GRB>(magnet, NUM_LEDS_MAG).setCorrection( TypicalLEDStrip );
  
  FastLED.setBrightness( BRIGHTNESS );
}

void Lights::handle() {
  for( int j = 0; j < 75; j++) {
    left[j] = CRGB::Blue;
  }

  for( int j = 75; j < NUM_LEDS_LEFT; j++) {
    left[j] = CRGB::Green;
  }
  
  for( int j = 0; j < NUM_LEDS; j++) {
      middle[j] = CRGB::Green;
      right[j] = CRGB::Green;
  }

  FastLED.show();
}
