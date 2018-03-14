#ifndef MIXER_H
#define MIXER_H
#include "palette_data.h"

// In this class we animate some faders or increments
// and then support mixing of the two renderbuffers into the frontbuffer
// Which then gets replicated (or not) and displayed
class Palmixer {
public:
  float fader[3] = {0.0f, 0.0f, 0.0f};    // Goes from 0.0f to 1.0f
  float deltaFade[3];                     // amount to fade per second
  bool active[3] = {false, false, false}; // are we currently animating anything?
  fract8 fraction[3];

  void Animate(float dt);
  void SetNewPalette(uint8_t whichSlot, uint8_t newPal, float seconds);
  enum PalChoice {
    kClouds = 0,
    kRainbow,
    kParty,
    kPurpleFly,
    kIris,
    kMadras,
    kLailah,
    kHope,
    kCompassion,
    kRepose,
    kMermaid,
    kSerendil,
    kFlame,
    kDaisyFae,
    kFaeCat,
    kFireIce,
    kHangOn,
    kMistress,
    kOtis,
    kScoutie,
    kSunlitWave,
    kTrove,
    kBlackhorse,
    kPlumbago,
    kSchwarzwald,
    kNrwc,
    kEpochs,
    kSky04,
    kSky12,
    kSky34,
    kSky44,
    kSky45,
    kSky05,
    kSky29,
    kRosa36,
    kRosa70,
    kVintage01,
    kLandscape27,
    kJul01,
    kIb63,
    kMango,
    kSprinkles,
    kNumPalettes
  };

  // An array of palette pointers so we can randomly choose one
  //#define kNumPalettes 64
  CRGBPalette16 palettes[kNumPalettes];

  // when we want to blend to a new palette, we assign to nextPalette, then use
  // nblend to modify the target palette over time.
  // all color fetch functions use the final palette in some way or another
  CRGBPalette256 nextPalette[3];
  CRGBPalette256 curPalette[3];
  CRGBPalette256 finalPalette[3];

  // This generates in mem copies of all of the progmem palettes that we actually want
  // These get upscaled into 256 entry palettes when assigned to nextPalette
  void GenerateGlobalPalettes();
};// } palmixer;

#endif
