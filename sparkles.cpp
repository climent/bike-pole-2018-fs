#include "effect.h"

Sparkles::Sparkles(int amount, int speed, bool white) {
  _amount = amount;
  _white = white;
  _speed = speed;
}

Sparkles::Sparkles(int amount, bool white) {
  _amount = amount;
  _white = white;
  _speed = 20;
}

void Sparkles::Animate(CRGB leds[NUM_LEDS]) {
  FadeAll(leds, _speed);
  CRGB color;
  if (random8(100) < _amount) {
    if (_white) {
      color = CRGB(255,255,255);
    } else {
      color = CRGB(random8(), random8(), random8());
    }
    leds[random8(NUM_LEDS)] = color;
  }
  // FastLED.show();
}

String Sparkles::Identify() {
  return "sparkles";
}
