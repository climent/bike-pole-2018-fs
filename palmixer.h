#ifndef MIXER_H
#define MIXER_H
#include "palettes.h"

// In this class we animate some faders or increments
// and then support mixing of the two renderbuffers into the frontbuffer
// Which then gets replicated (or not) and displayed
class Palmixer
{
  public:

  float fader[3] = {0.0f,0.0f,0.0f}; // Goes from 0.0f to 1.0f
  float deltaFade[3]; // amount to fade per second
  bool active[3] = {false,false,false}; // are we currently animating anything?
  fract8 fraction[3];

  void Animate(float dt) {};
  void SetNewPalette(uint8_t whichSlot, uint8_t newPal, float seconds) {};
  void SetRandomPalette(uint8_t whichSlot, float seconds) {};

} palmixer;

#endif
