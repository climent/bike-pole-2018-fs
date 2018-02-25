#include "FastLED.h"
#include "includes.h"
#include "effect.h"
#include "led_utils.h"

// Define the array of leds
CRGB leds[NUM_LEDS];

Effect* effects[1] = {
  new Blink(intLedPin)
};

void setup()
{
  Serial.begin(115200);
  Serial.println("resetting");
  LEDS.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);

  // pinMode(BTN_UP, INPUT_PULLUP);
  // digitalWrite(BTN_UP, HIGH);
  // pinMode(BTN_DW, INPUT_PULLUP);
  // digitalWrite(BTN_DW, HIGH);
}

void loop()
{
  effects[0]->Animate();
}
