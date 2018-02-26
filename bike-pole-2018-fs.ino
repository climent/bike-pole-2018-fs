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

#define PIN_UP 9
#define PIN_DOWN 11

Buttons buttons(PIN_UP, PIN_DOWN);

// Global Brightness
const uint8_t brightnessCount = 5;
uint8_t brightnessMap[brightnessCount] = { 16, 32, 64, 128, 255 };
uint8_t briLevel = 2;

void setup()
{
  Serial.begin(115200);
  Serial.println("resetting");
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds[2], NUM_LEDS);

  pinMode(PIN_UP, INPUT_PULLUP);
  digitalWrite(PIN_UP, HIGH);
  pinMode(PIN_DOWN, INPUT_PULLUP);
  digitalWrite(PIN_DOWN, HIGH);
}

int percent = 0;
void loop()
{
  // effects[1]->Animate(leds);
  // effects[0]->Animate(leds[0]);
  // effects[2]->Animate(leds[1]);
  // EVERY_N_SECONDS(1) {
  //   if (percent < 255) {
  //     percent += 5;
  //   } else {
  //     percent = 0;
  //   }
  // }
  // percent = 128;
  // for (int i = 0; i < NUM_LEDS; i++) {
  //   leds[2][i] = blend(leds[2][i], leds[0][i], 255);
  //   leds[2][i] = blend(leds[2][i], leds[1][i], 128);
  // }

  effects[1]->Animate(leds[2]);
  CheckBrightness();
  FastLED.show();
}

void CheckBrightness()
{
  int8_t button = buttons.Read();
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
