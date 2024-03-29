#include "effect.h"

void Bounce::Initialize() {
  now = millis();
  lastMove = millis();
  direction = true;
  position = 0;
  ended = false;
}

Bounce::Bounce(int speed, int tail) {
  _speed = speed;
  _tail = tail;
  Initialize();
}

Bounce::Bounce(){
  _speed = 5;
  _tail = 500;
  Initialize();
}

void Bounce::Animate() {
  now = millis();
  if (ended) ended = false;
  if (now - lastMove > _speed) {
    FadeAll(dst, _tail);
    dst[SetPixelsSingle(position)] = CHSV(hue++, 255, 255);
    // if direction == true, we are going up
    if (direction == true)
    {
      if (position == NUM_LEDS - 1)
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

void Bounce::Reset() {
  Initialize();
}
