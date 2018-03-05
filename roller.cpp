#include "effect.h"

void Roller::Reset() {
	Initialize();
}

void Roller::Initialize() {
	// initRoller = 0;
	height = NUM_LEDS / 3 - 1;
	now = millis();
	lastMove = millis();
	position = height;
	bottom = 0;
	ended = false;
}

Roller::Roller(CRGB fColor, CRGB bColor, int speed) {
	_fcolor = fColor;
	_bcolor = bColor;
	_speed = speed;
	Initialize();
}

Roller::Roller(CRGB fColor, CRGB bColor) {
	_fcolor = fColor;
	_bcolor = bColor;
	_speed = 200;
	Initialize();
}

void Roller::Animate() {
	now = millis();
  if (ended) ended = false;
	if (now - lastMove > _speed) {
		// FadeAll(leds, _tail);
		_leds = SetPixels(position);
		dst[_leds.o] = dst[_leds.p] = dst[_leds.q] = CRGB::Red;
		if (position + 1 <= height) {
			_leds = SetPixels(position + 1);
			dst[_leds.o] = dst[_leds.p] = dst[_leds.q] = CRGB::Black;
		}
		if (position == bottom)
		{
			position = height;
			if (bottom == height) {
				bottom = 0;
			} else {
				bottom++;
			}
      if (bottom == 1) ended = true;
		} else {
			position--;
		}
		lastMove = millis();
	}
}

bool Roller::CheckEnd() {
	return ended;
}

String Roller::Identify() {
	return "Roller";
}
