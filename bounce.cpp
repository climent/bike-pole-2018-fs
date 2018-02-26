#include "effect.h"

Bounce::Bounce(int speed, int tail) {
  now = millis();
  lastMove = millis();
  _speed = speed;
  direction = true;
  position = 0;
  _tail = tail;
}

void Bounce::Animate(CRGB leds[NUM_LEDS]) {
  now = millis();
  if (now - lastMove > _speed) {
    FadeAll(leds, _tail);
    _leds = SetPixels(position);
    leds[_leds.o] = leds[_leds.p] = leds[_leds.q] = CHSV(hue++, 255, 255);
    // if direction == true, we are going up
    if (direction == true)
    {
      if (position == NUM_LEDS/3 - 1)
      {
        direction = false;
        position--;
      } else {
        position++;
      }
    } else {
      if (position == 0)
      {
        direction = true;
        position++;
      } else {
        position--;
      }
    }
    // FastLED.show();
    lastMove = millis();
  }
}

String Bounce::Identify() {
  return "bounce";
}
