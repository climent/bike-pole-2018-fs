#ifndef COLOR_UTILS_H
#define COLOR_UTILS_H
#include <FastLED.h>

CHSV hsvOrange = CHSV(HUE_ORANGE, 255, 255);
CHSV hsvPurple = CHSV(HUE_PURPLE, 255, 255);
CHSV hsvBlue = CHSV(HUE_BLUE, 255, 255);
CHSV hsvRed = CHSV(HUE_RED, 255, 255);
CHSV hsvGreen = CHSV(HUE_GREEN, 255, 255);
CHSV hsvPink = CHSV(HUE_PINK, 255, 255);
CHSV hsvYellow = CHSV(HUE_YELLOW, 255, 255);

void PrintColor(CRGB c) {
  Serial.print("color: ");
  Serial.print(c.r);
  Serial.print(", ");
  Serial.print(c.g);
  Serial.print(", ");
  Serial.println(c.b);
}

void PrintColor(CHSV c) {
  Serial.print("hsvcolor: ");
  Serial.print(c.h);
  Serial.print(", ");
  Serial.print(c.s);
  Serial.print(", ");
  Serial.println(c.v);
}

void CheckColor(float r, float g, float b) {
  if (r < 0 || r > 255.0f)
  {
    Serial.print("Bad r value: "); Serial.println(r);
  }
  if (g < 0 || g > 255.0f)
  {
    Serial.print("Bad g value: "); Serial.println(g);
  }
  if (b < 0 || b > 255.0f)
  {
    Serial.print("Bad b value: "); Serial.println(b);
  }
}

CRGB randomcolor() {
  return CRGB(random(0, 256), random(0, 256), random(0, 256));
}

CHSV randomhsv() {
  return CHSV(random(0, 256), random(0, 256), random(0, 256));
}

/*! \brief Convert RGB to HSV color space

  Converts a given set of RGB values `r', `g', `b' into HSV
  coordinates. The input RGB values are in the range [0, 1], and the
  output HSV values are in the ranges h = [0, 360], and s, v = [0,
  1], respectively.

  \param fR Red component, used as input, range: [0, 1]
  \param fG Green component, used as input, range: [0, 1]
  \param fB Blue component, used as input, range: [0, 1]
  \param fH Hue component, used as output, range: [0, 360]
  \param fS Hue component, used as output, range: [0, 1]
  \param fV Hue component, used as output, range: [0, 1]

*/
void RGBtoHSV(float& fR, float& fG, float& fB,
              float& fH, float& fS, float& fV) {
  float fCMax = max(max(fR, fG), fB);
  float fCMin = min(min(fR, fG), fB);
  float fDelta = fCMax - fCMin;

  if (fDelta > 0) {
    if (fCMax == fR) {
      fH = 60 * (fmod(((fG - fB) / fDelta), 6));
    } else if (fCMax == fG) {
      fH = 60 * (((fB - fR) / fDelta) + 2);
    } else if (fCMax == fB) {
      fH = 60 * (((fR - fG) / fDelta) + 4);
    }

    if (fCMax > 0) {
      fS = fDelta / fCMax;
    } else {
      fS = 0;
    }

    fV = fCMax;
  } else {
    fH = 0;
    fS = 0;
    fV = fCMax;
  }

  if (fH < 0) {
    fH = 360 + fH;
  }
}

CHSV rgb2hsv(CRGB &in) {
  float r = (float)in.r / 255.0f;
  float g = (float)in.g / 255.0f;
  float b = (float)in.b / 255.0f;
  float h, s, v;
  RGBtoHSV(r, g, b, h, s, v);
  CHSV out;
  out.h = (int)((h / 360.0f) * 255.0f);
  out.s = (int)(s * 255.0f);
  out.v = (int)(v * 255.0f);
  return out;
}

#endif
