#include "effect.h"
#include "layout.h"

int Effect::SetPixelsSingle(int elevation) {
  return GetSinglePixelElevation(elevation);
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
