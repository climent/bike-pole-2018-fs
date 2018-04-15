#ifndef TIMER_H
#define TIMER_H

class Timer {
// Timer class just counts down to "event"
// When timer hits 0, event flag is set
// Event flag is only unset by fetching it, or by Reset
// Timer can either roll over and continue a new countdouwn, or just stop, depending on mode
public:
  enum TimerMode {
    ROLLOVER = 0,
    STOP = 1
  };

  unsigned long micsperevent = 1000000; // one per second by default
  long micstilevent = 0;
  bool eventTriggered = false;
  bool running = false;
  float frequency = 1.0f;
  TimerMode mode = ROLLOVER;
  
  Timer();
  void Reset();
  void SetMode(TimerMode m);
  void Start();
  void Stop();
  void SetFrequency(float f);
  void SetInterval(float seconds);
  void Update(unsigned long mics);
  bool GetEvent();
};

#endif
