#include "button.h"

Button::Button(int pin) {
  _pin = pin;
  millisBtwnPushes = 100;
  lastPush = millis();
  lastState = HIGH;
  pinMode(_pin, INPUT_PULLUP);
  digitalWrite(_pin, HIGH);
}

int8_t Button::Read() {
  now = millis();
  state = digitalRead(_pin);

  if (state == lastState) return 0;

  if (state != lastState && state == LOW)
  {
    if (now - millisBtwnPushes > lastPush)
    {
      lastPush = millis();
      if (DEBUG) Serial.println("button pressed");
      lastState = state;
      return 1;
    }
  } else {
    lastState = state;
  }
  return 0;
}

int Button::ReadNew() {
  int event = 0;
  // Read the state of the button
  buttonVal = digitalRead(_pin);
  // Button pressed down
  if (buttonVal == LOW && buttonLast == HIGH && (millis() - upTime) > debounce) {
    downTime = millis();
    ignoreUp = false;
    waitForUp = false;
    singleOK = true;
    holdEventPast = false;
    longHoldEventPast = false;
    if ((millis() - upTime) < DCgap && DConUp == false && DCwaiting == true) DConUp = true;
    else DConUp = false;
    DCwaiting = false;
  }
  // Button released
  else if (buttonVal == HIGH && buttonLast == LOW && (millis() - downTime) > debounce) {
    if (not ignoreUp) {
      upTime = millis();
      if (DConUp == false) DCwaiting = true;
      else {
        event = 2;
        DConUp = false;
        DCwaiting = false;
        singleOK = false;
        // eepromMillis = millis();
        // eepromOutdated = true;
      }
    }
  }
  // Test for normal click event: DCgap expired
  if ( buttonVal == HIGH && (millis() - upTime) >= DCgap && DCwaiting == true && DConUp == false && singleOK == true) {
    event = 1;
    DCwaiting = false;
    // eepromMillis = millis();
    // eepromOutdated = true;
  }
  // Test for hold
  if (buttonVal == LOW && (millis() - downTime) >= holdTime) {
    // Trigger "normal" hold
    if (not holdEventPast) {
      event = 3;
      waitForUp = true;
      ignoreUp = true;
      DConUp = false;
      DCwaiting = false;
      //downTime = millis();
      holdEventPast = true;
      // eepromMillis = millis();
      // eepromOutdated = true;
    }
    // Trigger "long" hold
    if ((millis() - downTime) >= longHoldTime) {
      if (not longHoldEventPast) {
        event = 4;
        longHoldEventPast = true;
        // eepromMillis = millis();
        // eepromOutdated = true;
      }
    }
  }
  buttonLast = buttonVal;
  return event;
}
