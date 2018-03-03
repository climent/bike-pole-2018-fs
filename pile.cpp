#include "effect.h"

void Pile::Reset() {
	Initialize();
}

void Pile::Initialize() {
	initPile = 0;
	height = NUM_LEDS / 3 - 1;
	now = millis();
	lastMove = millis();
	position = height;
	bottom = 0;
	ended = false;
}

Pile::Pile(CRGB fColor, CRGB bColor, int speed) {
	_fcolor = fColor;
	_bcolor = bColor;
	_speed = speed;
	Initialize();
}

Pile::Pile(CRGB fColor, CRGB bColor) {
	_fcolor = fColor;
	_bcolor = bColor;
	_speed = 200;
	Initialize();
}

void Pile::Animate() {
	now = millis();
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
				ended = true;
			} else {
				bottom++;
			}
		} else {
			position--;
		}
		lastMove = millis();
	}
}

bool Pile::CheckEnd() {
	return ended;
}

String Pile::Identify() {
	return "pile";
}
