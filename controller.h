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
  String GetBaseEffect();
  String GetLayerEffect();
  bool CheckEnd();
  void Reset();
  void SetBuffer(CRGB* dest);
  void Render();
  void Render(CRGBPalette256* finalPalette);
  // void Initialize();
  Effect* baseEffect;
  Effect* layerEffect;
  CRGB* base;
  CRGB* layer;
  CRGBPalette256* finalPalette;
private:
  int var;
};

#endif
