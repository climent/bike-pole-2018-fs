#include "effect.h"

PalTest::PalTest() {
}

void PalTest::Render() {
  fadeToBlackBy(dst, NUM_LEDS, 1);
  for (uint16_t j = 0; j < NUM_LEDS; j++) {
    int palindex = j;
    if (NUM_LEDS < 255) map(palindex, 0, NUM_LEDS, 0, 255);
    CRGB c = finalPalette[0][palindex];
    dst[SetPixelsSingle(j)] = c;
  }
}

String PalTest::Identify() {
  return "paltest";
}
