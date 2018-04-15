#include "effect.h"

String Null::Identify() {
  return "null effect";
}

void Null::Animate() {
  fadeToBlackBy(dst, NUM_LEDS, 255);
}

// void Render() {
//   FadeOrClear();
// }
