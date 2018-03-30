#ifndef PALMIXER_H
#define PALMIXER_H
#include <FastLED.h>

// In this class we animate some faders or increments
// and then support mixing of the two renderbuffers into the frontbuffer
// Which then gets replicated (or not) and displayed
class Palmixer {
public:
  Palmixer(
    CRGBPalette16* palettes,
    CRGBPalette256* nextPalette,
    CRGBPalette256* currentPalette,
    CRGBPalette256* finalPalette
    );
  float fader[3] = {0.0f, 0.0f, 0.0f};    // Goes from 0.0f to 1.0f
  float deltaFade[3];                     // amount to fade per second
  fract8 fraction[3];
  void UpdatePalettes(int deltaMicros);
  void Animate(float mics);
  void SetNewPalette(uint8_t whichSlot, uint8_t newPal, float seconds);
  void SetTimer(int timeTilPalChange);
private:
  // An array of palette pointers so we can randomly choose one
  int _kNumPalettes;
  CRGBPalette16* _palettes;

  // when we want to blend to a new palette, we assign to nextPalette, then use
  // nblend to modify the target palette over time.
  // all color fetch functions use the final palette in some way or another
  CRGBPalette256* _nextPalette;
  CRGBPalette256* _currentPalette;
  CRGBPalette256* _finalPalette;
  bool active[3] = {false, false, false}; // are we currently animating anything?
  int timer;
  int timeLeftTilPalChange;
};

#endif
