#include "Arduino.h"
#include "Lights.h"
#include "logic.h"
#include <FastLED.h>

#define CHIPSET       WS2812B
#define NUM_LEDS      75
#define NUM_LEDS_LEFT 174
#define NUM_LEDS_MAG  13

#define BRIGHTNESS  100

CRGB left[NUM_LEDS_LEFT];
CRGB middle[NUM_LEDS];
CRGB right[NUM_LEDS];

CRGB magnet[NUM_LEDS_MAG];

#define MAGNET_DELAY 700
unsigned long magnet_on_at = 0;
uint8_t magnet_delta = 0;
unsigned long previous_breath = 0;
static bool doneBreathing = false;
uint8_t flame_height = 0;

Lights::Lights(Logic &logic)
: _logic(logic)
{  
}

void Lights::moveToLevel(int level) {
  int l = level - 1;
  flame_height = l * 10;

  // SOLVED!
  if (l >= 7) {
    flame_height = NUM_LEDS;
    changeBowl(true);

    // magnet light gets triggered by logic after a wait time
  }
  else {
    // make sure bowl and magnet are reset
    changeBowl(false);
    changeMagnet(false);

    // not easy to spread 75 across 7, so even numbers we add 1
    if (l > 0 && l % 2 == 0) {
      flame_height++;
    }

    // means we went down, need to reset
    if (_level > 0 && level < _level) {
      _logic.serial.print("lights: level went down, resetting\r\n");
      _fade = true;
    } else {
      _fade = false;
    }

    _level = level;
  }
}

void Lights::fire(uint16_t top, uint16_t cl, uint16_t sp)
{  
    // instead of a strict copy to black when they reset, lets fade it to black
    if (_fade) {
      bool allBlack = true;
      for(int i = 0; i < NUM_LEDS; i++) {
        if (left[i].r != 0 || left[i].g != 0 || left[i].b != 0 || 
            middle[i].r != 0 || middle[i].g != 0 || middle[i].b != 0 ||
            right[i].r != 0 || right[i].g != 0 || right[i].b != 0 )
        {
          allBlack = false;
        }
        
        left[i].fadeToBlackBy(64);
        middle[i].fadeToBlackBy(64);
        right[i].fadeToBlackBy(64);
      }

      if (!allBlack){
        return;
      } else {
        _fade = false;
      }
    }

    // Array of temperature readings at each simulation cell
    static byte hL[NUM_LEDS];
    static byte hM[NUM_LEDS];
    static byte hR[NUM_LEDS];

    // Step 1.  Cool down every cell a little
    for(int i = 0; i < top; i++) {
      hL[i] = qsub8(hL[i],  random8(0, ((cl * 10) / top) + 2));
      hM[i] = qsub8(hM[i],  random8(0, ((cl * 10) / top) + 2));
      hR[i] = qsub8(hR[i],  random8(0, ((cl * 10) / top) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k = top - 1; k >= 2; k--) {
      hL[k] = (hL[k - 1] + hL[k - 2] + hL[k - 2] ) / 3;
      hM[k] = (hM[k - 1] + hM[k - 2] + hM[k - 2] ) / 3;
      hR[k] = (hR[k - 1] + hR[k - 2] + hR[k - 2] ) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if(random8() < sp) {
      int y = random8(7);
      hL[y] = qadd8(hL[y], random8(160,255) );
    }
    if(random8() < sp) {
      int y = random8(7);
      hM[y] = qadd8(hM[y], random8(160,255) );
    }
    if(random8() < sp) {
      int y = random8(7);
      hR[y] = qadd8(hR[y], random8(160,255) );
    }

   // Step 4.  Map from heat cells to LED colors
    for(int j = 0; j < top; j++) {
      CRGB cL = ColorFromPalette(HeatColors_p, scale8(hL[j], 240));
      CRGB cM = ColorFromPalette(HeatColors_p, scale8(hM[j], 240));
      CRGB cR = ColorFromPalette(HeatColors_p, scale8(hR[j], 240));

      left[j] = cL;
      middle[j] = cM;
      right[j] = cR;
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
  if (!show) {
    for( int j = 75; j < NUM_LEDS_LEFT; j++) {
      left[j] = CRGB::Black;
    }
  }
}

void Lights::setup() {
  // 22 = left
  // 23 = middle
  // 21 = right
  FastLED.addLeds<CHIPSET, 22, GRB>(left, NUM_LEDS_LEFT).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, 23, GRB>(middle, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, 21, GRB>(right, NUM_LEDS).setCorrection( TypicalLEDStrip );

  FastLED.addLeds<CHIPSET, 17, GRB>(magnet, NUM_LEDS_MAG).setCorrection( TypicalLEDStrip );
  
  FastLED.setBrightness( BRIGHTNESS );

  // initial state is level 7, high before they start the puzzle
  moveToLevel(7);
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

void Lights::bowl() {
  static CRGBPalette16 currentPalette = CRGBPalette16(CRGB::Black, CRGB::Red, CRGB::Orange);
  static uint8_t startIndex = 0;
  startIndex = startIndex + 2; /* motion speed */

  uint8_t colorIndex = startIndex;
  for( int i = NUM_LEDS; i < NUM_LEDS_LEFT; i++) {
    left[i] = ColorFromPalette(currentPalette, colorIndex, 255, LINEARBLEND);
    colorIndex += 3;
  }
}

void Lights::handle() {
  random16_add_entropy(esp_random());

  if (magnet_on_at > 0 && millis() - magnet_on_at > MAGNET_DELAY) {
    breath(15);
  }

  fire(flame_height, 55, 120);
  
  if (bowlOn) {
    bowl();
  }

  FastLED.show();
  FastLED.delay(1000 / 60);
}
