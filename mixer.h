#include <FastLED.h>

#ifndef MIXER_H
#define MIXER_H

class Mixer {
public:
  Mixer();
  Mixer(CRGB* base_buffer, CRGB* layer_buffer, CRGB* mixed_buffer);
  void Mix();

  CRGB* _base = NULL;
  CRGB* _layer = NULL;
  CRGB* _mixed = NULL;

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
  int percent;
private:
  int _percent;
};

#endif
