#include "effect.h"

int lowerBlock = 30;
int upperBlock = 40;

// Returns a triplet of pixels for a single pixel elevation, turning then
// entire pole into a single elevation.
Leds Effect::SetPixels(uint16_t elevation) {
	Leds leds;
	if (elevation < lowerBlock) {
		leds.o = elevation;
		leds.p = lowerBlock * 2 - elevation - 1;
		leds.q = lowerBlock * 2 + elevation;
	} else if (elevation >= lowerBlock) {
		leds.o = lowerBlock * 2 + elevation;
		leds.p = lowerBlock * 4 + upperBlock * 2 - elevation - 1;
		leds.q = lowerBlock * 2 + upperBlock * 2 + elevation;
	}
	return leds;
}

uint16_t Effect::SetPixelsSingle(uint16_t elevation) {
	if (elevation < NUM_LEDS) return elevation;

	if (elevation < lowerBlock)
			return elevation; // o
	if (elevation >= lowerBlock && 
		  elevation < lowerBlock + upperBlock) 
			return lowerBlock * 4 + upperBlock * 2 - elevation - 1; // p'

	if (elevation >= lowerBlock + upperBlock && 
			elevation < lowerBlock * 2 + upperBlock)
			return lowerBlock * 2 - elevation - 1; // p
	if (elevation >= lowerBlock * 2 + upperBlock &&
			elevation < lowerBlock * 2 + upperBlock * 2) 
			return lowerBlock * 2 + upperBlock * 2 + elevation; // o'

	if (elevation >= lowerBlock * 2 + upperBlock * 2 && 
			elevation < lowerBlock * 3 + upperBlock * 2)
			return elevation + lowerBlock * 2; // q
	if (elevation >= lowerBlock * 3 + upperBlock * 2 &&
			elevation < lowerBlock * 3 + upperBlock * 3)
			return elevation + 3 * lowerBlock; // q'
	// return -1;
	return 0;
}

uint16_t Effect::SetPixel(uint16_t elevation) {
	if (_singleString) {
    if (elevation < NUM_LEDS) return elevation;
		return -1;
	} else {
		return -1;
	}
}

CRGB Effect::SetColorByPalette(CRGBPalette256* finalPalette, int pal,
    uint16_t palindex) {
#ifdef TEST_BOARD
#endif

#ifdef CARDBOARD_TUBE
	if (palindex < NUM_LEDS / 2) {
		palindex = map(palindex, 0, NUM_LEDS / 2, 0, 255);
		CRGB c = finalPalette[pal][palindex];
		return c;
	}
	palindex = map(palindex, NUM_LEDS / 2, NUM_LEDS, 255, 0);
	CRGB c = finalPalette[pal][palindex];
	return c;
#endif
	}

void Effect::FadeAll(CRGB leds[NUM_LEDS], int fade) {
	for (uint16_t i = 0; i < NUM_LEDS; i++) {
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
    for (uint16_t i = 0; i < NUM_LEDS; i++)
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
