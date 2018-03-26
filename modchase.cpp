#ifndef MODCHASE_H
#define MODCHASE_H

#include "effect.h"

Modchase::Modchase() {
}

String Modchase::Identify() {
  return "modchase";
}

void Modchase::Reset() {
  micspershift = (unsigned long)((1.0f/30) * (float)1000000);
  timeTillShift = 0;

  micsperlaunch = 100000;
  timeTillLaunch = 0;

  palIndex = 0;
  numToShift = 0;
  numToEmit = 0;
}

// void SetSpeed(float ledspersec) {
//   effect::SetSpeed(ledspersec);
//   // How many mics to wait til next change
//   micspershift = (unsigned long)((1.0f/ledspersec) * (float)ONEMIL);
//   timeTillShift = micspershift;
// }

// void SetFrequency(float f)
// {
//   effect::SetFrequency(f);
//   // How many mics to wait til next launch
//   micsperlaunch = (unsigned long)((1.0f/f) * (float)ONEMIL);
//   timeTillLaunch = micsperlaunch;
// }

void Modchase::Animate(unsigned long mics) {
  timeTillShift -= mics;
  timeTillPalChange -= mics;
  timeTillLaunch -= mics;

  // Move the patterns when it is time
  if (timeTillShift <= 0) {
    numToShift++;
    timeTillShift = micspershift;
  }

  // Launch when it is time
  if (timeTillLaunch <= 0) {
    numToEmit = (int)60;
    timeTillLaunch = micsperlaunch;
  }

  if (timeTillPalChange <= 0) {
    palIndex += 1;
    timeTillPalChange = micsperpalchange;
  }

  // Scroll
  while (numToShift > 0) {
    for (int i = NUM_LEDS - 1; i >= 0; i--)
    {
      int from = i - 1;
      if (from >= 0)
        dst[SetPixelsSingle(i)] = dst[SetPixelsSingle(from)];
      else
        dst[SetPixelsSingle(i)] = CRGB::Black;
    }

    // for (int i = NUM_LEDS / 2; i < NUM_LEDS; i++)
    // {
    //   int from = i + 1;
    //   if (from < NUM_LEDS)
    //     dst[i] = dst[from];
    //   else
    //     dst[i] = CRGB::Black;
    // }

    numToShift--;

    // Emit new stuff
    if (numToEmit > 0)
    {
      // Use palette later
      //dst[0] = finalPalette[pal][palIndex++]; // Just march the palette for now
      dst[0] = finalPalette[pal][palIndex];
      // dst[NUM_LEDS - 1] = finalPalette[pal][palIndex + 64];
      numToEmit--;
    }
  }
}

// Have the two spirals shift past one another
uint8_t extra = 0;

void Modchase::Render()
{
  // //FadeOrClear();
  // extra++;
  //
  // // Scroll
  // while (numToShift > 0) {
  //   for (int i = NUM_LEDS / 2 - 1; i >= 0; i--)
  //   {
  //     int from = i - 1;
  //     if (from >= 0)
  //       dst[i] = dst[from];
  //     else
  //       dst[i] = CRGB::Black;
  //   }
  //
  //   for (int i = NUM_LEDS / 2; i < NUM_LEDS; i++)
  //   {
  //     int from = i + 1;
  //     if (from < NUM_LEDS)
  //       dst[i] = dst[from];
  //     else
  //       dst[i] = CRGB::Black;
  //   }
  //
  //   numToShift--;
  //
  //   // Emit new stuff
  //   if (numToEmit > 0)
  //   {
  //     // Use palette later
  //     //dst[0] = finalPalette[pal][palIndex++]; // Just march the palette for now
  //     dst[SetPixelsSingle(0)] = finalPalette[pal][palIndex++];
  //     dst[SetPixelsSingle(NUM_LEDS-1)] = finalPalette[pal][palIndex+64];
  //     numToEmit--;
  //   }
  // }
}

#endif
