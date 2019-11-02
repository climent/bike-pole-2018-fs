#include "effect.h"

PalTest::PalTest() {
}

void PalTest::Render() {
  fadeToBlackBy(dst, NUM_LEDS, 1);
  for (uint16_t j = 0; j < NUM_LEDS; j++) {
    CRGB c = SetColorByPalette(finalPalette, pal, j);
    dst[SetPixelsSingle(j)] = c;
  }
}

String PalTest::Identify() {
  return "paltest";
}
