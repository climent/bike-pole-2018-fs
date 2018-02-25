#include "includes.h"
#include <FastLED.h>

#ifndef EFFECT_H
#define EFFECT_H

class Effect {
public:
  virtual void Animate(CRGB leds[NUM_LEDS]);
  virtual String Identify();

  Leds SetPixels(int elevation);
  void FadeAll(CRGB leds[NUM_LEDS], int fade);
};

class Flash : public Effect {
public:
  Flash();
  Flash(CRGB color);
  void Animate(CRGB leds[NUM_LEDS]);
  String Identify();
private:
  CRGB _color;
  bool hasColor;
  long now;
  bool ledOn;
  long timeToFlash;
  long lastFlash;
  int  numberOfFlashes;
  int  elevation;
  uint8_t hue;
  Leds _leds;
  int _totalFlashes;
  long flashTime;
  long lastShortFlash;
  void Initialize();
};

class Bounce : public Effect {
public:
  Bounce(int speed, int tail);
  void Animate(CRGB leds[NUM_LEDS]);
  String Identify();
private:
  long now;
  long _speed;
  long lastMove;
  bool direction;
  int position;
  uint8_t hue;
  Leds _leds;
  int _tail;
};

class Sparkles : public Effect {
public:
  Sparkles(int amount, bool white);
  Sparkles(int amount, int speed, bool white);
  void Animate(CRGB leds[NUM_LEDS]);
  String Identify();
private:
  int _amount;
  int _white;
  int _speed;
};

#endif
