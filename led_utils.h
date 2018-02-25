#ifndef LED_UTILS_H
#define LED_UTILS_H

void SetThreePixels(CRGB* leds, int pos, CHSV chsv)
{
  Leds l;
  if (pos < 30)
  {
    l.o = pos;
    l.p = 60 - pos - 1;
    l.q = pos + 60;
  }
  if (pos >= 30)
  {
    l.o = pos + 60;
    l.p = 200 - pos - 1;
    l.q = pos + 140;
  }
  leds[l.o] = leds[l.p] = leds[l.q] = chsv;
}

#endif
