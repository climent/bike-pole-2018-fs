#include "includes.h"
#include <FastLED.h>

#ifndef BUTTONS_H
#define BUTTONS_H

class Buttons {
public:
  Buttons(int pin_up, int pin_down);
  Buttons(int pin_change);
  int8_t Read();
private:
  int _pin_up;
  int _pin_down;
  int millisBtwnPushes;
  int lastPush;
  int lastUpState;
  int upState;
  int lastDwState;
  int dwState;
  int lastChangeState;
  int changeState;
  int now;
  int _pin_change;
  bool single_button;
};

#endif
