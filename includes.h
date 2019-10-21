// This FASTLED_INTERNAL is only here to avoid warning messages.
#define FASTLED_INTERNAL 1

// How many leds in your strip?
#define NUM_LEDS 288
// #define NUM_LEDS 120
// #define DATA_PIN 7
// #define NUM_LEDS 64 // test board

// Button pins for up and down
#define PIN_UP 9
#define PIN_DOWN 11
#define PIN_EFFECT 6 //17
// #define PIN_EFFECT 16 // test board

#ifndef STRUCTS_H
#define STRUCTS_H
#include <FastLED.h>

#define DEBUG 1
#define MOTION 0
#define USEMIXER 0
#define MAX_DIMENSION 16

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
