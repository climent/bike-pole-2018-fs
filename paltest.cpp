#include "effect.h"

PalTest::PalTest(int _inc) {
  length = _inc;
}

void PalTest::Render() {
  uint16_t level = 0;
  int blocks = 8;
  fadeToBlackBy(dst, NUM_LEDS, 1);
  for (int block = 0; block < blocks; block++){    
    for (uint16_t j = 0; j < length; j++) {
      if (level < NUM_LEDS) {
          CRGB c = finalPalette[0][level];
          dst[SetPixelsSingle(level)] = c;
          level++;
      }
    }
  }
}

String PalTest::Identify() {
  return "paltest";
}
