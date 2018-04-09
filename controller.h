#include "includes.h"
#include "effect.h"
#include <FastLED.h>

#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller : public Effect {
public:
  Controller(CRGB* baseLeds, CRGB* layerLeds);
  Controller(CRGB* baseLeds, CRGB* layerLeds, CRGB* nextBaseLeds, CRGB* nextLayerLeds);
  String Identify();
  void Animate(unsigned long mics);
  void SetEffect(Effect* effect);
  void SetBaseEffect(Effect* effect);
  void SetLayerEffect(Effect* effect);
  String GetBaseEffect();
  String GetLayerEffect();
  void SetNextBaseEffect(Effect* effect);
  void SetNextLayerEffect(Effect* effect);
  String GetNextBaseEffect();
  String GetNextLayerEffect();
  bool CheckEnd();
  void Reset();
  void SetBuffer(CRGB* dest);
  void SetOutputBuffer(CRGB* dest);
  bool Render(int deltaMillis);
  void SetTimer(int timer);

  Effect* baseEffect;
  Effect* layerEffect;
  CRGB* baseBuffer;
  CRGB* layerBuffer;
  CRGB* outputBuffer;
  Effect* nextBaseEffect;
  Effect* nextLayerEffect;
  CRGB* nextBaseBuffer;
  CRGB* nextLayerBuffer;
  CRGBPalette256* finalPalette;
  CRGBPalette256* nextFinalPalette;


private:
  float fader[2] = {0.0f, 0.0f};
  float deltaFade[2] = {1.0f / 4, 1.0f / 4}; // amount to fade per second
  fract8 fraction[2] = {0, 0};

  int var;
  int timer;
  // int timeLeftTilEffectChanges;

  int timeLeftTilRender;
  int timeTilRender;
};

#endif
