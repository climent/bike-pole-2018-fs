#include "includes.h"
#include "effect.h"
#include <FastLED.h>

#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller : public Effect {
public:
  Controller();
  String Identify();
  void Animate();
  void SetEffect(Effect* effect);
  Effect* _effect;
  // void Animate(unsigned long mics);
  // void SetEffect(Effect* effect);
  // void SetBaseEffect(effect* effect);
  // void SetLayerEffect(effect* effect);
  bool CheckEnd();
  void Reset();
private:
  // void
  int var;
};

#endif
