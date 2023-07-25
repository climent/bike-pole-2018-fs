#include "effect.h"

Sparkles::Sparkles(int amount, int speed, bool white) {
  _amount = amount;
  _white = white;
  _speed = speed;
  ended = true;
}

Sparkles::Sparkles(int amount, bool white) {
  _amount = amount;
  _white = white;
  _speed = 20;
  ended = true;
}

void Sparkles::Animate() {
  FadeAll(dst, _speed);
  CRGB color;
  if (random8(100) < _amount) {
    if (_white) {
      color = CRGB(255,255,255);
    } else {
      color = CRGB(random8(), random8(), random8());
    }
    dst[random8(NUM_LEDS)] = color;
  }
}

String Sparkles::Identify() {
  return "sparkles";
}
