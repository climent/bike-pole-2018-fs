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

  uint16_t SetPixelElevation(uint16_t elevation);
  uint16_t SetPixelsSingle(uint16_t elevation);
  // uint16_t SetPixel(uint16_t elevation);

  CRGB SetColorByPalette(
    CRGBPalette256* finalPalette,
    int pal,
    uint16_t palindex);

  void FadeAll(CRGB leds[NUM_LEDS], int fade);
  void FadeOrClear();
  void SetPalette(CRGBPalette256* finalPalette);
  void SetPaleteIndex(uint8_t pal);

  bool waitToEnd;
  bool ended = true;
  uint8_t pal = 0; // pal index 0, 1 or 2
  CRGBPalette256* finalPalette = NULL;
private:
  bool _singleString = false;
};

class Null : public Effect {
public:
  void Animate();
  String Identify();
private:
};

class Noise : public Effect {
public:
  Noise();
  void Animate(unsigned long mics);
  String Identify();
  void Initialize();
  void Reset();
  // void Render(CRGBPalette256* palette);
  void Render();
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
  // Leds _leds;
  int _totalFlashes;
  long flashTime;
  long lastShortFlash;
};

class Bounce : public Effect {
public:
  Bounce();
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
  CRGB _fcolor;
  CRGB _bcolor;
  int _speed;
  int initPile;
  int height;
  int now;
  int lastMove;
  int position;
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
  CRGB _fcolor;
  CRGB _bcolor;
  int _speed;
  int height;
  int now;
  int lastMove;
  int position;
  int bottom;
};

class Plasma : public Effect {
public:
  Plasma();
  void Animate(unsigned long mics);
  String Identify();
  void Initialize();
  void Reset();
  // void Render(CRGBPalette256* palette);
  void Render();
private:
  void SlowVels();
  float nodes[9]; // units are frontpalette space (0-255)
  float vels[9];  // in frontpalette units per second
  int locs[9]; // the node locations in strip space
  int m_length;
};

class Modchase : public Effect {
public:
  Modchase();
  String Identify();
  void Reset();
  void SetSpeed(float ledspersec);
  void SetFrequency(float f);
	void Animate(unsigned long mics);
	void Render();
  unsigned long micspershift = (unsigned long)((1.0f/30) * (float)1000000);
  int timeTillShift = 0;
  unsigned long micsperlaunch = 1800000;
  int timeTillLaunch = 0;
  unsigned long micsperpalchange = 150000;
  int timeTillPalChange = 0;
  unsigned char palIndex = 0;
  int numToShift = 0;
  int numToEmit = 0;
};

class Twinkles : public Effect {
public:
  Twinkles(int amount, bool white);
  Twinkles(int amount, int speed, bool white);
  String Identify();
  void Animate(unsigned long mics);
  void Render();
private:
  void Twinkle();
  int _amount;
  int _white;
  int _speed;
  const CRGB peak_color = CRGB(150, 150, 150);
  int ledState[NUM_LEDS];
  CRGB ledColors[NUM_LEDS] = {CRGB::Black};
  enum {SteadyDim, GettingBrighter, GettingDimmerAgain};
  long timeToTinkle = 0;
};

class PalTest : public Effect {
public:
  PalTest();
  String Identify();
  void Render();
private:
};

#endif
