#include "includes.h"
#include "effect.h"
#include <FastLED.h>

#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller : public Effect {
public:
  Controller(CRGB* baseLeds, CRGB* layerLeds);
  String Identify();
  void Animate(unsigned long mics);
  void SetEffect(Effect* effect);
  void SetBaseEffect(Effect* effect);
  void SetLayerEffect(Effect* effect);
  Effect* baseEffect;
  Effect* layerEffect;
  CRGB* base;
  CRGB* layer;
  // void Animate(unsigned long mics);
  // void SetEffect(Effect* effect);
  // void SetBaseEffect(effect* effect);
  // void SetLayerEffect(effect* effect);
  bool CheckEnd();
  void Reset();
  void SetBuffer(CRGB* dest);
private:
  // void
  int var;
};

#endif
