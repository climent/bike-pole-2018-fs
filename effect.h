#include "includes.h"
#include <FastLED.h>

#ifndef EFFECT_H
#define EFFECT_H

class Effect {
public:
  virtual String Identify();
  // Animates the effect based on the timing
  virtual void Animate();
  virtual void Animate(unsigned long mics);
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
  virtual void Initialize();
  virtual void Render();
  virtual void Render(CRGBPalette256* finalPalette);
  Leds SetPixels(int elevation);
  int SetPixelsSingle(int elevation);
  void FadeAll(CRGB leds[NUM_LEDS], int fade);
  void FadeOrClear();

  bool waitToEnd;
  bool ended;
  uint8_t pal; // pal index 0, 1 or 2
};

class Null : public Effect {
public:
private:
};

class Noise : public Effect {
public:
  Noise();
  void Animate(unsigned long mics);
  String Identify();
  void Initialize();
  void Reset();
  void Render(CRGBPalette256* palette);
private:
  // static const int ONEMIL = 1000000;
  unsigned long micsperemit = 1000000;
  int timeTillEmit = 0;

  // The 16 bit version of our coordinates
  uint16_t x;
  uint16_t y;
  uint16_t z;

  // We're using the x/y dimensions to map to the x/y pixels on the matrix.  We'll
  // use the z-axis for "time".  speed determines how fast time moves forward.  Try
  // 1 for a very slow moving effect, or 60 for something that ends up looking like
  // water.
  uint16_t speed = 10; // speed is set dynamically once we've started up

  // Scale determines how far apart the pixels in our noise matrix are.  Try
  // changing these values around to see how it affects the motion of the display.  The
  // higher the value of scale, the more "zoomed out" the noise iwll be.  A value
  // of 1 will be so zoomed in, you'll mostly see solid colors.
  uint16_t scale = 30; // scale is set dynamically once we've started up

  // This is the array that we keep our computed noise values in
  uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];
  uint8_t startIndex = 0; // this is where in the palette we start, cycle this slowly to 255 and wrap around
  uint8_t colorLoop = 1;
  CRGBPalette256* palette = NULL;
  void FillNoise8();
  void MapNoiseToLEDsUsingPalette(CRGBPalette256* palette);
};

class Flash : public Effect {
public:
  Flash();
  Flash(CRGB color);
  void Animate();
  String Identify();
  void SetColor(CRGB color);
  void Initialize();
private:
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
  void Initialize();
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
  void Animate();
  String Identify();
private:
  int _amount;
  int _white;
  int _speed;
};

class Pile : public Effect {
public:
  Pile();
  Pile(CRGB fColor, CRGB bColor);
  Pile(CRGB fColor, CRGB bColor, int speed);
  void Reset();
  void Animate(unsigned long mics);
  String Identify();
  bool CheckEnd();
  void Initialize();
private:
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

class Roller : public Effect {
public:
  Roller(CRGB fColor, CRGB bColor);
  Roller(CRGB fColor, CRGB bColor, int speed);
  void Reset();
  void Animate();
  String Identify();
  bool CheckEnd();
  void Initialize();
private:
  int _fcolor;
  int _bcolor;
  int _speed;
  int height;
  int now;
  int lastMove;
  int position;
  Leds _leds;
  int bottom;
};

#endif
