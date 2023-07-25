#ifndef LED_UTILS_H
#define LED_UTILS_H

// Some utils to get normed values of height and angle (around staff) based on led index i
float GetHeight(int i)
{
  float p = (float)(i) / (float)(NUM_LEDS);
  if (p <= 0.5f)
  {
    return (p * 2.001f);
  }
  else
  {
    return (1.0f - p) * 2.001f;
  }
}

// 0.0 - 1.0 not radians, or degrees, just normed coordinate around the circle
float GetAngle(int i)
{
  float numLedsPerThread = 8.0f;

  float p = 0.0f;
  if (i >= NUM_LEDS / 2)
  {
    i = NUM_LEDS - i;
    p = (float)(i) / numLedsPerThread;
    p += 0.5f; // other  side
  }
  else
  {
    p = (float)(i) / numLedsPerThread;
  }

  while (p > 1.0f)
    p -= 1.0f;
  return p;
}

#endif


