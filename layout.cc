#include "layout.h"

// This is the file that should contain all the special code that controls how
// the LEDs are configured in shape and form.

// Essential functions are:
//  - Elevation(): returns the number of LEDs for effects that need to know how
//                 tall the effect should go to. E.g., 120 LEDs that are divided
//                 into 3 different strings, the elevation is effectively 40.
//  - 

int lowerBlock = 30;
int upperBlock = 40;

// Returns a triplet of pixels for a single pixel elevation, turning then
// entire pole into a single elevation.
Leds SetPixels(uint16_t elevation) {
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

uint16_t Elevation() {
  return NUM_LEDS;
  return elevation;
}
