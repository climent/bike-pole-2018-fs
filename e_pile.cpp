#include "effect.h"

void Pile::Reset() {
	Initialize();
}

void Pile::Initialize() {
	// initPile = 0;
	height = NUM_LEDS - 1;
	now = millis();
	lastMove = millis();
	position = height;
	bottom = 0;
	ended = false;
	_speed = 5;
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

void Pile::Animate(unsigned long mics) {
	// unsigned long milliseconds = mics * 1000;
	now = millis();
  if (ended) ended = false;
	if (now - lastMove > _speed) {
		// FadeAll(leds, _tail);
		dst[SetPixelsSingle(position)] = _fcolor;
		dst[SetPixelsSingle(position + 1)] = _fcolor;
		dst[SetPixelsSingle(position + 2)] = _fcolor;
		if (position + 3 <= height) {
			dst[SetPixelsSingle(position + 3)] = _bcolor;
		}
		if (position <= bottom)
		{
			position = height;
			if (bottom >= height) {
				bottom = 0;
			} else {
				bottom++;
				bottom++;
				bottom++;
			}
      if (bottom == 1) ended = true;
		} else {
			position--;
			// position--;
		}
		lastMove = millis();
		// if (DEBUG) Serial.print("lastmove: ");
		// if (DEBUG) Serial.println(lastMove);
		// if (DEBUG) Serial.print("position: ");
		// if (DEBUG) Serial.println(position);
	}
}

bool Pile::CheckEnd() {
	return ended;
}

String Pile::Identify() {
	return "pile";
}
