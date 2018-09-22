#include "effect.h"

int lowerBlock = 30;
int upperBlock = 40;

// Returns a triplet of pixels for a single pixel elevation, turning then
// entire pole into a single elevation.
Leds Effect::SetPixels(int elevation) {
	Leds leds;
	if (elevation < lowerBlock) {
		leds.o = elevation;
		leds.p = lowerBlock * 2 - elevation - 1;
		leds.q = elevation + lowerBlock * 2;
	} else if (elevation >= lowerBlock) {
		leds.o = elevation + lowerBlock * 3;
		leds.p = lowerBlock * 4 + upperBlock * 2 - elevation - 1;
		leds.q = elevation + lowerBlock * 2 + upperBlock * 2;
	}
	return leds;
}

int Effect::SetPixelsSingle(int elevation) {
	if (elevation < 30) return elevation;
	if (elevation >= 30 && elevation < 70) return 200 - 1 - elevation;
	if (elevation >= 70 && elevation < 100) return 130 - 1 - elevation;
	if (elevation >= 100 && elevation < 140) return 70 + elevation;
	if (elevation >= 140 && elevation < 170) return elevation - 80;
	if (elevation >= 170 && elevation < 210) return elevation - 170 + 90;
	return -1;
}

void Effect::FadeAll(CRGB leds[NUM_LEDS], int fade) {
	for (int i = 0; i < NUM_LEDS; i++) {
		leds[i].nscale8(fade);
	}
}

bool Effect::CheckEnd() {
	return true;
}

void Effect::Initialize() {
	// Override in inheriting classes.
}

void Effect::Animate(unsigned long mics) {
  Animate();
}

void Effect::Render() {
	//Overrided in inheriting classes.
}

void Effect::Animate() {
	// Override in inheriting classes.
}

void Effect::FadeOrClear() {
  // if (clearmode == kClear) {
  //   Blackout(dst);
  // } else {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      dst[i].nscale8(100);
    }
  // }
}

void Effect::SetPaleteIndex(uint8_t palIndex) {
	pal = palIndex;
}

void Effect::SetPalette(CRGBPalette256* palette) {
	finalPalette = palette;
}
