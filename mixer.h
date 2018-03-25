#ifndef MIXER_H
#define MIXER_H
#include "includes.h"
#include <FastLED.h>

class Mixer {
public:
  Mixer();
  Mixer(CRGB* base_buffer, CRGB* layer_buffer, CRGB* mixed_buffer);
  Mixer(CRGB* mixed_buffer);
  void SetCurrentBuffer(CRGB* primaryBuffer);
  void SetSecondary(CRGB* secondaryBuffer);
  void MixToPrimary(CRGB* buffer, int time);
  CRGB* GetCurrentBuffer();
  void Animate(float mics);
  bool IsActive();
  void Mix();

  CRGB* _base = NULL;
  CRGB* _layer = NULL;
  CRGB* currentBuffer = NULL;
  CRGB* nextBuffer = NULL;
  CRGB* mixedBuffer = NULL;

  enum mixMode {
    kOverwrite = 0,
    kBlendHalf = 1,
    kWriteIfNonBlack = 2,
    kBlendIfBrighter = 3,
    kOverwriteIfEven = 4,
    kChooseBrightest = 5,
    kOverwriteIfBrighter = 6,
    kOff = 7,
  };
private:
  bool active;
  float fader = 0.0f;
  float deltaFade;
  fract8 fraction;
};

#endif
