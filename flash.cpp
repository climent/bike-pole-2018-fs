#include "effect.h"

void Flash::Initialize() {
  now = millis();
  ledOn = false;
  timeToFlash = 2000;
  lastFlash = millis();
  numberOfFlashes = 3;
  hue = 0;
  _totalFlashes = 0;
  flashTime = 15;
  lastShortFlash = millis();
}

Flash::Flash() {
  Initialize();
  hasColor = false;
}

Flash::Flash(CRGB color){
  Initialize();
  hasColor = true;
  _color = color;
}

void Flash::Blink(CRGB leds[NUM_LEDS]) {
  now = millis();
  if (now - lastShortFlash > flashTime){
    if (!ledOn){
      for (int i = 0; i < NUM_LEDS / 3; i++){
        _leds = SetPixels(i);
        if (hasColor){
          leds[_leds.o] = leds[_leds.p] = leds[_leds.q] = _color;
        } else {
          leds[_leds.o] = leds[_leds.p] = leds[_leds.q] = CHSV(hue++, 255, 80);
        }
      }
    } else {
      for (int i = 0; i < NUM_LEDS; i++){
        leds[i] = CHSV(0, 0, 0);
      }
      _totalFlashes += 1;
    }
    lastShortFlash = millis();
    ledOn = !ledOn;
    // Serial.println(_totalFlashes);
    if (_totalFlashes >= numberOfFlashes) {
      lastFlash = millis();
      _totalFlashes = 0;
    }
  }
}

void Flash::Animate(CRGB leds[NUM_LEDS]) {
  now = millis();
  if (now - lastFlash > timeToFlash) {
    Blink(leds);
  }
}

String Flash::Identify() {
  return "flash";
}
