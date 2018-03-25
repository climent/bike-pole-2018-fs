#include "effect.h"
#include "led_utils.h"
// #include "palmixer.h"
// This example combines two features of FastLED to produce a remarkable range of
// effects from a relatively small amount of code.  This example combines FastLED's
// color palette lookup functions with FastLED's Perlin/simplex noise generator, and
// the combination is extremely powerful.
//
// You might want to look at the "ColorPalette" and "Noise" examples separately
// if this example code seems daunting.
//
//
// The basic setup here is that for each frame, we generate a new array of
// 'noise' data, and then map it onto the LED matrix through a color palette.
//
// Periodically, the color palette is changed, and new noise-generation parameters
// are chosen at the same time.  In this example, specific noise-generation
// values have been selected to match the given color palettes; some are faster,
// or slower, or larger, or smaller than others, but there's no reason these
// parameters can't be freely mixed-and-matched.
//
// In addition, this example includes some fast automatic 'data smoothing' at
// lower noise speeds to help produce smoother animations in those cases.
//
// The FastLED built-in color palettes (Forest, Clouds, Lava, Ocean, Party) are
// used, as well as some 'hand-defined' ones, and some proceedurally generated
// palettes.

//#define MAX_DIMENSION ((kMatrixWidth>kMatrixHeight) ? kMatrixWidth : kMatrixHeight)
#define MAX_DIMENSION 16

// Create some "boards" to do image and fluid and flame effects into
static const int kMatrixWidth = 4;
static const int kMatrixHeight = 70;
unsigned char board[kMatrixHeight][kMatrixWidth];
unsigned char work[kMatrixHeight][kMatrixWidth];

Noise::Noise() {
  // Initialize();
}

void Noise::Reset() {
  Initialize();
}

void Noise::Initialize()
{
  // Initialize our coordinates to some random values
  x = random16();
  y = random16();
  z = random16();
}

String Noise::Identify() {
  return "noise";
}

void Noise::Animate(unsigned long mics)
{
  // timeTillEmit -= mics;
  //
  // // Move the patterns when it is time
  // if (timeTillEmit <= 0)
  // {
  //   timeTillEmit = micsperemit;
  //   // FillNoise8();
  // }
  FillNoise8();
}

void Noise::Render()
{
  FadeOrClear();
  MapNoiseToLEDsUsingPalette(finalPalette);
}

// Fill the x/y array of 8-bit noise values using the inoise8 function.
void Noise::FillNoise8() {

  // unsigned long startmics = micros();

  // If we're runing at a low "speed", some 8-bit artifacts become visible
  // from frame-to-frame.  In order to reduce this, we can do some fast
  // data-smoothing.
  // The amount of data smoothing we're doing depends on "speed".
  uint8_t dataSmoothing = 0;
  if (speed < 50) {
    dataSmoothing = 200 - (speed * 4);
  }

  for(int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = scale * i;
    for(int j = 0; j < MAX_DIMENSION; j++) {
      int joffset = scale * j;

      uint8_t data = inoise8(x + ioffset,y + joffset,z);

      // The range of the inoise8 function is roughly 16-238.
      // These two operations expand those values out to roughly 0..255
      // You can comment them out if you want the raw noise data.
      data = qsub8(data,16);
      data = qadd8(data,scale8(data,39));

      if (dataSmoothing) {
        uint8_t olddata = noise[i][j];
        uint8_t newdata = scale8(olddata, dataSmoothing) + scale8(data, 256 - dataSmoothing);
        data = newdata;
      }

      noise[i][j] = data;
    }
  }

  z += speed;

  // apply slow drift to X and Y, just for visual variation.
  x += speed / 8;
  y -= speed / 16;

  // unsigned long endmics = micros();
  // unsigned int millis = (endmics-startmics)/1000;
  // Serial.printf("Noise function takes %d milliseconds\n",millis);
}

void Noise::MapNoiseToLEDsUsingPalette(CRGBPalette256* palette)
{
  for (int i = 0; i < NUM_LEDS; i++) {
    // Each led has a height and width that we can use to find the best render value
    int y = (int)(GetHeight(i) * kMatrixHeight) % MAX_DIMENSION;
    int x = ((int)(GetAngle(i) * kMatrixWidth) + (y/4)) % MAX_DIMENSION;

    // We use the value at the (i,j) coordinate in the noise
    // array for our brightness, and the flipped value from (j,i)
    // for our pixel's index into the color palette.

    uint8_t index = noise[x][y];
    uint8_t bri =   noise[y][x];

    // brighten up, as the color palette itself often contains the
    // light/dark dynamic range desired
    if( bri > 127 ) {
      bri = 255;
    } else {
      bri = dim8_raw(bri * 2);
    }

    // The palette holder has 3 slots for palettes. So far we are using just one
    // but if we want to use several, update this variable.
    int pal = 1;

    index += startIndex;
    CRGB c = finalPalette[pal][index];
    c.nscale8(bri);
    dst[SetPixelsSingle(i)] = c;
  }
  startIndex++;
}
