// How many leds in your strip?
#define NUM_LEDS 210
// #define DATA_PIN 7

// Button pins for up and down
#define PIN_UP 9
#define PIN_DOWN 11
#define PIN_EFFECT 17

#ifndef STRUCTS_H
#define STRUCTS_H
#include "FastLED.h"

#define DEBUG 1
#define MOTION 0
#define MAX_DIMENSION 32

/*
         o
    ############### -     ->###############
         p           |   |
  - ###############<-     - ###############<-
 |       q                                   |
  ->############### ------->############### -
*/
struct Leds {
  int o, p, q;
};
typedef struct Leds Leds;

#endif
