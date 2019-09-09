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

#define MAGNET_DELAY 2000
bool magnetLightOn = false;
unsigned long magnet_on_at = 0;
uint8_t magnet_delta = 0;
unsigned long previous_breath = 0;
static bool doneBreathing = false;
bool bowlOn = false;

Lights::Lights(Logic &logic)
: _logic(logic)
{  
}

void Lights::moveToLevel(int level) {
  int l = level - 1;
  int upperBound = l * 10;

  // SOLVED!
  if (l >= 7) {
    upperBound = NUM_LEDS;
    changeBowl(true);
    changeMagnet(true);
  }
  else {
    // make sure bowl and maget are reset
    changeBowl(false);
    changeMagnet(false);
  }

  // not easy to spread 75 across 7, so even numbers we add 1
  if (l > 0 && l % 2 == 0) {
    upperBound++;
  }

  for( int j = 0; j < NUM_LEDS; j++) {
    CRGB c =  j < upperBound ? CRGB::Green : CRGB::Black;

    left[j] = c;
    middle[j] = c;
    right[j] = c;
  }
}

void Lights::toggleMagnet() {
  changeMagnet(!magnetLightOn);
}

void Lights::changeMagnet(bool show) {
  magnetLightOn = show;
  doneBreathing = false;
  magnet_on_at = show ? millis() : 0;

  if (!show) {
    magnet_delta = 0;
    previous_breath = 0;
    fill_solid(magnet, NUM_LEDS_MAG, CRGB::Black);
  }
}

void Lights::toggleBowl() {
  changeBowl(!bowlOn);
}

void Lights::changeBowl(bool show) {
  bowlOn = show;
  for( int j = 75; j < NUM_LEDS_LEFT; j++) {
    left[j] = show ? CRGB::Blue : CRGB::Black;
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

void Lights::breath(uint16_t interval) {
  const uint8_t min_brightness = 35;

  if ( !doneBreathing && (millis() - previous_breath) > interval ) {
        previous_breath = millis();
        uint8_t max_brightness = 220;
        uint8_t b = scale8(triwave8(magnet_delta), max_brightness-min_brightness)+min_brightness;

        for( int j = 0; j < NUM_LEDS_MAG; j++) {
          magnet[j] = CHSV(0, 0, b);
        }

        if (b == max_brightness - 1) {
          doneBreathing = true;
        }

        magnet_delta++;
  }
}

void Lights::handle() {
  if (magnet_on_at > 0 && millis() - magnet_on_at > MAGNET_DELAY) {
    breath(15);
  }

  FastLED.show();
}
