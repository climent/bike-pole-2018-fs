#include "effect.h"

void Flash::Initialize() {
  ledOn = false;
  timeToFlash = 2000;
  lastFlash = millis();
  numberOfFlashes = 3;
  hue = 0;
  _totalFlashes = 0;
  flashTime = 15;
  lastShortFlash = millis();
  ended = true;
}

Flash::Flash() {
  Initialize();
  hasColor = false;
}

Flash::Flash(CRGB color) {
  Initialize();
  hasColor = true;
  _color = color;
}

void Flash::SetColor(CRGB color) {
  _color = color;
}

void Flash::Blink() {
  now = millis();
  if (now - lastShortFlash > flashTime){
    if (!ledOn){
      for (int i = 0; i < NUM_LEDS / 3; i++){
        _leds = SetPixels(i);
        if (hasColor){
          dst[_leds.o] = dst[_leds.p] = dst[_leds.q] = _color;
        } else {
          dst[_leds.o] = dst[_leds.p] = dst[_leds.q] = CHSV(hue++, 255, 80);
        }
      }
    } else {
      for (int i = 0; i < NUM_LEDS; i++){
        dst[i] = CHSV(0, 0, 0);
      }
      _totalFlashes += 1;
    }
    lastShortFlash = millis();
    ledOn = !ledOn;
    if (_totalFlashes >= numberOfFlashes) {
      lastFlash = millis();
      _totalFlashes = 0;
    }
  }
}

void Flash::Animate() {
  now = millis();
  // Serial.println(now);
  if (now - lastFlash > timeToFlash) {
    Blink();
  }
}

String Flash::Identify() {
  return "flash";
}
