#include "effect.h"

void Pile::Reset() {
	Initialize();
}

void Pile::Initialize() {
	// initPile = 0;
	height = NUM_LEDS / 3 - 1;
	now = millis();
	lastMove = millis();
	position = height;
	bottom = 0;
	ended = false;
	_speed = 10;
	_fcolor = CRGB::Red;
	_bcolor = CRGB::Black;
}

Pile::Pile(CRGB fColor, CRGB bColor, int speed) {
	Initialize();
	_fcolor = fColor;
	_bcolor = bColor;
	_speed = speed;
}

Pile::Pile(CRGB fColor, CRGB bColor) {
	Initialize();
	_fcolor = fColor;
	_bcolor = bColor;
}

Pile::Pile() {
	Initialize();
}

void Pile::Render() {
	for (int i = 0; i < NUM_LEDS; i++) {
		dst[i] = ledColors[i];
	}
}

void Pile::Animate(unsigned long mics) {
	// unsigned long milliseconds = mics * 1000;
	now = millis();
  if (ended) ended = false;
	if (now - lastMove > _speed) {
		// FadeAll(leds, _tail);
		_leds = SetPixels(position);
		ledColors[_leds.o] = ledColors[_leds.p] = ledColors[_leds.q] = _fcolor;
		if (position + 1 <= height) {
			_leds = SetPixels(position + 1);
			ledColors[_leds.o] = ledColors[_leds.p] = ledColors[_leds.q] = _bcolor;
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

bool Pile::CheckEnd() {
	return ended;
}

String Pile::Identify() {
	return "pile";
}
