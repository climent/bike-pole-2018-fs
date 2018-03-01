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

Buttons::Buttons(int pin_change) {
  _pin_change = pin_change;
  single_button = true;
  millisBtwnPushes = 100;
  lastPush = millis();
  lastChangeState = HIGH;
  pinMode(_pin_change, INPUT_PULLUP);
  digitalWrite(_pin_change, HIGH);

}

Buttons::Buttons(int pin_up, int pin_down) {
  _pin_up = pin_up;
  _pin_down = pin_down;
  millisBtwnPushes = 100;
  lastPush = millis();
  lastUpState = HIGH;
  lastDwState = HIGH;
  pinMode(pin_up, INPUT_PULLUP);
  digitalWrite(pin_up, HIGH);
  pinMode(pin_down, INPUT_PULLUP);
  digitalWrite(pin_down, HIGH);
}

int8_t Buttons::Read() {
  now = millis();
  if (!single_button) {
    upState = digitalRead(_pin_up);
    dwState = digitalRead(_pin_down);

    // Serial.print(lastUpState); Serial.print("  "); Serial.println(upState);
    // Serial.println(dwState);
    if (upState == lastUpState && dwState == lastDwState) return 0;

    if (upState != lastUpState && upState == LOW)
    {
      if (now - millisBtwnPushes > lastPush)
      {
        lastPush = millis();
        Serial.println("up");
        lastUpState = upState;
        return 1;
      }
    } else {
      lastUpState = upState;
    }

    if (dwState != lastDwState && dwState == LOW)
    {
      if (now - millisBtwnPushes > lastPush)
      {
        lastPush = millis();
        Serial.println("down");
        lastDwState = dwState;
        return -1;
      }
    } else {
      lastDwState = dwState;
    }
  } else {
    changeState = digitalRead(_pin_change);

    if (changeState == lastChangeState) return 0;

    if (changeState != lastChangeState && changeState == LOW)
    {
      if (now - millisBtwnPushes > lastPush)
      {
        lastPush = millis();
        Serial.println("change");
        lastChangeState = changeState;
        return 1;
      }
    } else {
      lastChangeState = changeState;
    }
  }
  return 0;
}

#endif
