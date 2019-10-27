#ifndef PALMIXER_H
#define PALMIXER_H

#define FASTLED_INTERNAL 1
#include <FastLED.h>

// In this class we 
// 4 different sets of data, passed to the
// constructorIn this class we animate some faders or increments
// and then support mixing of the two renderbuffers into the frontbuffer
// Which then gets replicated (or not) and displayed
class Palmixer {
public:
  Palmixer(
    CRGBPalette16* palettes,
    CRGBPalette256* currentPalette,
    CRGBPalette256* nextPalette,
    CRGBPalette256* finalPalette
    );
  void Initialize();
  float fader[3] = {0.0f, 0.0f, 0.0f};    // Goes from 0.0f to 1.0f
  float deltaFade[3];                     // amount to fade per second
  fract8 fraction[3];
  void UpdatePalettes(int deltaMicros);
  void Animate(float mics);
  void SetTimer(long timeTilPalChange);
  void SetChanger(bool changer);
  bool GetChanger();
  void SetTransitionTimer(float transitionTimer);
  void SetDefaultPalette(int defaultPalette);
  CRGBPalette256 GetPalette();  // defaults to palette in slot 0
  CRGBPalette256 GetPalette(int8_t index);
  int GetPaletteIndex();
  void SetRandomPalettes(bool randomly);
private:
  void SetNewPalette(uint8_t whichSlot, uint8_t newPal, float seconds);
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
  long timer = 1000000;
  long timeLeftTilPalChange;
  float seconds = 4.0f;
  int defaultPalette = 0;
  bool randomly = true;
  int incrementalPalette = 0;
};

#endif
