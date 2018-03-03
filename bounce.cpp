#include "effect.h"

Bounce::Bounce(int speed, int tail) {
  now = millis();
  lastMove = millis();
  _speed = speed;
  direction = true;
  position = 0;
  _tail = tail;
  ended = false;
}

void Bounce::Animate() {
  now = millis();
  if (ended) ended = false;
  if (now - lastMove > _speed) {
    FadeAll(dst, _tail);
    _leds = SetPixels(position);
    dst[_leds.o] = dst[_leds.p] = dst[_leds.q] = CHSV(hue++, 255, 255);
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
        ended = true;
      } else {
        position--;
      }
    }
    // FastLED.show();
    lastMove = millis();
  }
}

bool Bounce::CheckEnd() {
	return ended;
}

String Bounce::Identify() {
  return "bounce";
}
