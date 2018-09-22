#include "effect.h"

Test::Test(int _length, CRGB _color) {
  length = _length;
  color = _color;
}

void Test::Render() {
  // fadeToBlackBy(dst, NUM_LEDS, 1);
  for ( uint16_t i = 0; i < length; i++) {
    dst[i] = color;
  }  
}

String Test::Identify() {
  return "test";
}
