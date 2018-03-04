#include "includes.h"
#include <FastLED.h>

#ifndef EFFECT_H
#define EFFECT_H

class Effect {
public:
  virtual String Identify();
  // Animates the effect based on the timing
  virtual void Animate();
  // Renders the animation into the LED array
  // virtual void Render(CRGB leds[NUM_LEDS]);

  CRGB* dst = NULL;

  virtual void SetBuffer(CRGB* dest) {
    dst = dest;
  }; // Must have a destination to render

  virtual CRGB* GetBuffer() {
    return dst;
  }; // Get the working buffer for this effect

  virtual void Reset() {};

  virtual bool CheckEnd();

  Leds SetPixels(int elevation);
  void FadeAll(CRGB leds[NUM_LEDS], int fade);

  bool waitToEnd;
  bool ended;
};

class Flash : public Effect {
public:
  Flash();
  Flash(CRGB color);
  void Animate();
  String Identify();
  void SetColor(CRGB color);
private:
  void Initialize();
  void Blink();
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
};

class Bounce : public Effect {
public:
  Bounce(int speed, int tail);
  void Animate();
  String Identify();
  void Reset();
  bool CheckEnd();
private:
  void Initialize();
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
  void Animate();
  String Identify();
private:
  int _amount;
  int _white;
  int _speed;
};

class Pile : public Effect {
public:
  Pile(CRGB fColor, CRGB bColor);
  Pile(CRGB fColor, CRGB bColor, int speed);
  void Reset();
  void Animate();
  String Identify();
  bool CheckEnd();
private:
  void Initialize();
  int _fcolor;
  int _bcolor;
  int _speed;
  int initPile;
  int height;
  int now;
  int lastMove;
  int position;
  Leds _leds;
  int bottom;
};

#endif
