#include "effect.h"
#include "layout.h"

uint16_t Effect::SetPixelsSingle(uint16_t elevation) {
 	if (elevation < NUM_LEDS) return elevation;
  return GetSinglePixelElevation(elevation);
}
CRGB Effect::SetColorByPalette(CRGBPalette256* finalPalette, int pal,
    uint16_t palindex) {
	palindex = map(palindex, 0, NUM_LEDS, 0, 255);
	CRGB c = finalPalette[pal][palindex];
	return c;
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

void Effect::Render(CRGBPalette256* finalPalette) {
	//Overrided in inheriting classes.
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
  //   for (int i = 0; i < NUM_LEDS; i++)
  //   {
  //     dst[i].nscale8(fadeunits);
  //   }
  // }
}

void Effect::SetPaleteIndex(uint8_t palIndex) {
	pal = palIndex;
}

void Effect::SetPalette(CRGBPalette256* palette) {
	finalPalette = palette;
}
