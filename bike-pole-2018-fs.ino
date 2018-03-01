#include <FastLED.h>
#include "includes.h"
#include "effect.h"
#include "led_utils.h"
#include "buttons.h"

#define DATA_PIN 7

// Define the array of leds
// buffer 0 and 1 are for mainaining the
CRGB leds[3][NUM_LEDS];

Effect *effects[] = {
  new Flash(CRGB::Red),
  new Bounce(20, 220),
  new Sparkles(80, 5, true),
};

Buttons briButtons = Buttons(PIN_UP, PIN_DOWN);
Buttons effectButtons = Buttons(PIN_EFFECT);

// Global Brightness
const uint8_t brightnessCount = 5;
uint8_t brightnessMap[brightnessCount] = { 16, 32, 64, 128, 255 };
uint8_t briLevel = 2;
uint8_t currentEffect = 0;

// Timers


void setup()
{
  Serial.begin(115200);
  Serial.println("resetting");
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds[2], NUM_LEDS);
}

void loop()
{
  effects[currentEffect]->Animate(leds[2]);
  CheckBrightness();
  CheckEffect();
  FastLED.show();
}

void CheckEffect() {
  int8_t button = effectButtons.Read();
  if (button == 1) {
    currentEffect += 1;
    if (currentEffect > 2) currentEffect = 0;
  }
}

void CheckBrightness() {
  int8_t button = briButtons.Read();
  switch (button) {
    case 1:
      if (briLevel < 4) briLevel += 1;
      break;
    case -1:
      if (briLevel > 0) briLevel -= 1;
      break;
  }
  LEDS.setBrightness(brightnessMap[briLevel]);
}
