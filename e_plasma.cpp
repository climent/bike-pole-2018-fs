#ifndef PLASMA_H
#define PLASMA_H

#include "effect.h"

//#define MAX_DIMENSION ((kMatrixWidth>kMatrixHeight) ? kMatrixWidth : kMatrixHeight)
#define MAX_DIMENSION 32

void Plasma::Reset() {
  Initialize();
}

void Plasma::Initialize()
{
  m_length = NUM_LEDS;
  // For now, random vels
  for (int i = 0; i < 8; i++) {
    int r = random(10,255);
    vels[i] = (float)(r);
    locs[i] = i * (m_length / 8);
    nodes[i] = 0;
  }
  locs[8] = m_length - 1; // put last node at the end
}

Plasma::Plasma() {
  Initialize();
}

String Plasma::Identify() {
  return "plasma";
}

void Plasma::SlowVels() {
  // For now, random vels
  int v[9] = {255, 1024, 0, 255, 128, 255, 0, 1024, 255};
  for (int i = 0; i < 9; i++) {
    int n;
    if (i % 2) {
      n = 0;
    } else {
      n = 255;
    }
    vels[i] = (float)(v[i]);
    nodes[i] = (float)(n);
  }
}

void Plasma::Animate(unsigned long mics) {
 	float dt = (float)(mics) / (float)1000000.0f;

  for (int i = 0; i < 8; i++) {
    nodes[i] += vels[i] * dt;
    if (nodes[i] < 0) {
      nodes[i] = - nodes[i]; // bounce
      vels[i] = - vels[i];
    }
    if (nodes[i] > (float)(255)) {
      nodes[i] = (float)(511) - nodes[i]; // bounce
      vels[i] = - vels[i];
    }
  }
  // if (DEBUG) {
  //   EVERY_N_SECONDS(1) {
  //     Serial.print(F("nodes: ["));
  //     for (int m = 0; m < 9; m++) {
  //       Serial.print(nodes[m]);
  //       Serial.print(F(", "));
  //     }
  //     Serial.println(F("]"));
  //   }
  // }
}

void Plasma::Render() {
  FadeOrClear();
  for (int i = 0; i < 8; i++)
  {
    // interpolate from node i to i+1 in frontpalette space
    float startp = nodes[i];
    float endp = nodes[i + 1];
    float pdelta = (endp - startp) / (float)(locs[i + 1] - locs[i]);
    float curp = startp;
    // if (DEBUG) {
    //   EVERY_N_SECONDS(1) {
    //     Serial.print(F("curp: ["));
    //     Serial.print(curp);
    //     Serial.println(F("]"));
    //   }
    // }
    for (int l = locs[i]; l <= locs[i + 1]; l++) {
      int palindex = (int)(curp);
      if (palindex < 0) palindex = 0;
      if (palindex > 255) palindex = 255;
      //dst[l] = finalPalette[pal][palindex];
      int pal = 1;
      dst[SetPixelsSingle(l)] = finalPalette[pal][palindex];
      curp += pdelta;
    }
  }
}

#endif
