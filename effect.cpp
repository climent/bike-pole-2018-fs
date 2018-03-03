#include "effect.h"

Leds Effect::SetPixels(int elevation) {
	Leds leds;
	if (elevation < 30) {
		leds.o = elevation;
		leds.p = 60 - elevation - 1;
		leds.q = elevation + 60;
	} else if (elevation >= 30) {
		leds.o = elevation + 60;
		leds.p = 200 - elevation - 1;
		leds.q = elevation + 140;
	}
	return leds;
}

void Effect::FadeAll(CRGB leds[NUM_LEDS], int fade) {
	for (int i = 0; i < NUM_LEDS; i++) {
		leds[i].nscale8(fade);
	}
}
