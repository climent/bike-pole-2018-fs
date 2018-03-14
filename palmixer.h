#ifndef MIXER_H
#define MIXER_H
#include "FastLED.h"

// In this class we animate some faders or increments
// and then support mixing of the two renderbuffers into the frontbuffer
// Which then gets replicated (or not) and displayed
class Palmixer {
public:
  Palmixer(int kNumPalettes, CRGBPalette16* palettes);
  float fader[3] = {0.0f, 0.0f, 0.0f};    // Goes from 0.0f to 1.0f
  float deltaFade[3];                     // amount to fade per second
  bool active[3] = {false, false, false}; // are we currently animating anything?
  fract8 fraction[3];
  int kNumPalettes;
  CRGBPalette16 palettes[];

  void Animate(float dt);
  void SetNewPalette(uint8_t whichSlot, uint8_t newPal, float seconds);

  // An array of palette pointers so we can randomly choose one
  // #define kNumPalettes 64
  // CRGBPalette16 palettes[kNumPalettes];

  // when we want to blend to a new palette, we assign to nextPalette, then use
  // nblend to modify the target palette over time.
  // all color fetch functions use the final palette in some way or another
  CRGBPalette256 nextPalette[3];
  CRGBPalette256 curPalette[3];
  CRGBPalette256 finalPalette[3];

};

#endif
