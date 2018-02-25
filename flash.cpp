#include "effect.h"

Flash::Flash() {
  now = millis();
  ledOn = false;
  timeToFlash = 500;
  lastFlash = millis();
  numberOfFlashes = 3;
  hue = 0;
}

void Flash::Animate(CRGB leds[NUM_LEDS]){
  now = millis();
  // Serial.println(now);
  if (now - lastFlash > timeToFlash) {
    if (!ledOn){
      for (int i = 0; i < NUM_LEDS / 3; i++){
        _leds = SetPixels(i);
        leds[_leds.o] = leds[_leds.p] = leds[_leds.q] = CHSV(hue++, 255, 80);
      }
      ledOn = !ledOn;
    } else {
      for (int i = 0; i < NUM_LEDS; i++){
        leds[i] = CHSV(0, 0, 0);
      }
      ledOn = !ledOn;
    }
    // FastLED.show();
    lastFlash = millis();
  }
}

String Flash::Identify() {
  return "flash";
}
