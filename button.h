#include "includes.h"
#include <FastLED.h>

#ifndef BUTTON_H
#define BUTTON_H

class Button {
public:
  Button(int pin);
  bool Read();
  int ReadEvent();
private:
  int _pin;
  int millisBtwnPushes;
  int lastPush;
  int lastState;
  int state;

  unsigned long now;
  unsigned long lastCycle = millis();
  unsigned long eepromMillis;
  bool    eepromOutdated;

  // Button timing variables
  unsigned int debounce = 20; // ms debounce period to prevent flickering when pressing or releasing the button
  unsigned int DCgap = 250; // max ms between clicks for a double click event
  unsigned int holdTime = 1500; // ms hold period: how long to wait for press+hold event
  unsigned int longHoldTime = 5000; // ms long hold period: how long to wait for press+long hold event

  // Other button variables
  boolean buttonVal =          HIGH; // value read from button
  boolean buttonLast =         HIGH; // buffered value of the button's previous state
  boolean DCwaiting =         false; // whether we're waiting for a double click (down)
  boolean DConUp =            false; // whether to register a double click on next release, or whether to wait and click
  boolean singleOK =           true; // whether it's OK to do a single click
  long    downTime =             -1; // time the button was pressed down
  long    upTime =               -1; // time the button was released
  boolean ignoreUp =          false; // whether to ignore the button release because the click+hold was triggered
  boolean waitForUp =         false; // when held, whether to wait for the up event
  boolean holdEventPast =     false; // whether or not the hold event happened already
  boolean longHoldEventPast = false; // whether or not the long hold event happened already
};

#endif
