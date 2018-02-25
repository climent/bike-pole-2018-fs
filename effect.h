// #include "includes.h"
#include <FastLED.h>

#ifndef EFFECT_H
#define EFFECT_H

class Effect
{
public:
  virtual void Animate();
  virtual String Identify();
};

class Blink : public Effect
{
public:
  Blink(int i);
  void Animate();
  String Identify();
private:
  int _ledPin;
  long now;
  long timeToChange;
  long lastChange;
  bool ledOn;
};



#endif
