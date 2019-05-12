#ifndef POOLS_H
#define POOLS_H
// #include "commonheaders.h"
#include "effect.h"

Pools::Pools() {
  m_length = NUM_LEDS;
  // For now, random vels
  for (int i = 0; i < 9; i++)
  {
    int r = random(10, 255);
    vels[i] = (float)(r);
    // TODO(): check what changes when we change the 9
    locs[i] = i * (m_length / 2); 
    nodes[i] = 0;
  }
  locs[8] = m_length - 1; // put last node at the end
}

String Pools::Identify() {
  return "pools";
}

void Pools::SlowVels() {
  // For now, random vels
  for (int i = 0; i < 9; i++)
  {
    int r = random(50, 200);
    vels[i] = (float)(r);
  }
}

void Pools::FastVels() {
  // For now, random vels
  for (int i = 0; i < 9; i++)
  {
    int r = random(100, 400);
    vels[i] = (float)(r);
  }
}

void Pools::Rythmic() {
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

void Pools::Animate(unsigned long mics) {
	float dt = (float)(mics) / (float)1000000.0f;

  for (int i = 0; i < 9; i++) {
    nodes[i] += vels[i] * dt;
    if (nodes[i] < 0) {
     nodes[i] = -nodes[i]; // bounce
     vels[i] = -vels[i];
    }
    if (nodes[i] > (float)(255)) {
      nodes[i] = (float)(511) - nodes[i]; // bounce
      vels[i] = -vels[i];
    }
  }
}

void Pools::Render() {
  FadeOrClear();

  bool output_single_string = true;

  for (int i = 0; i < 8; i++) {
    // interpolate from node i to i+1 in frontpalette space
    float startp = nodes[i];
    float endp = nodes[i + 1];
    float pdelta = (endp-startp) / (float)(locs[i + 1] - locs[i]);
    float curp = startp;
    for (int l = locs[i]; l <= locs[i + 1]; l++)
    {
      int palindex = (int)(curp);
      if (palindex < 0) palindex = 0;
      if (palindex > 255) palindex = 255;
      if (output_single_string) {
        dst[l] = finalPalette[pal][palindex];
      } else {
        dst[SetPixelsSingle(l)] = finalPalette[pal][palindex];
      }
      curp += pdelta;
    }
  }
}

#endif
