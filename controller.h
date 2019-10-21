#include "includes.h"
#include "effect.h"
#include <FastLED.h>

#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller : public Effect {
public:
  Controller(
      CRGB* baseLeds,
      CRGB* layerLeds);
  Controller(
      CRGB* baseLeds,
      CRGB* layerLeds,
      CRGB* nextBaseLeds,
      CRGB* nextLayerLeds);
  Controller(
      CRGB* baseLeds,
      CRGB* layerLeds,
      CRGB* nextBaseLeds,
      CRGB* nextLayerLeds,
      CRGB* outputLeds);
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
  bool Render(unsigned long deltaMillis);
  void SetTimer(long timer);
  void InitiateTransition(Effect* effect);
  void InitiateTransition(Effect* effect, bool fast);
  bool IsTransitionActive();

  Effect* baseEffect;
  Effect* layerEffect;
  CRGB* baseBuffer;
  CRGB* layerBuffer;
  Effect* nextBaseEffect;
  Effect* nextLayerEffect;
  CRGB* nextBaseBuffer;
  CRGB* nextLayerBuffer;

  CRGB* outputBuffer;

  // CRGBPalette256* finalPalette;
  // CRGBPalette256* nextFinalPalette;

private:
  void PreRender(unsigned long deltaMillis);
  void Mix();

  bool transitionActive = false;

  float fader[2] = {0.0f, 0.0f};
  float deltaFade[2] = {1.0f / 5, 1.0f / 5}; // amount to fade per second
  fract8 fraction[2] = {0, 0};

  int var;
  int timer;

  long timeLeftTilRender;
  long timeTilRender;
};

#endif
