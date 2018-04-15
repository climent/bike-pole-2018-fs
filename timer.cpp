#include "timer.h"

Timer::Timer() {
}

// enum TimerMode {
//   ROLLOVER = 0,
//   STOP = 1
// };

// unsigned long micsperevent = ONEMIL; // one per second by default
// long micstilevent = 0;
// bool eventTriggered = false;
// bool running = false;
// float frequency = 1.0f;
// TimerMode mode = ROLLOVER;

void Timer::Reset() {
  micstilevent = micsperevent;
  eventTriggered = false;
}

void Timer::SetMode(TimerMode m) {
  mode = m;
}

void Timer::Start() {
  running = true;
}

void Timer::Stop() {
  running = false;
}

void Timer::SetFrequency(float f) {
  // How many mics to wait til next event
  frequency = f;
  micsperevent = (unsigned long)((1.0f/f) * 1000000.0f);
}

void Timer::SetInterval(float seconds) {
  frequency = 1.0f / seconds;
  micsperevent = (unsigned long)(seconds * 1000000.0f);
}

void Timer::Update(unsigned long mics) {
  if (!running) return;
  micstilevent -= mics;
  if (micstilevent <= 0) {
    micstilevent += micsperevent;
    eventTriggered = true;
    if (mode == STOP) {
      micstilevent = micsperevent;
      running = false;
    }
  }
}

bool Timer::GetEvent() {
  bool retVal = eventTriggered;
  if (retVal)
    eventTriggered = false;
  return retVal;
}
