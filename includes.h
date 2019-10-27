// This FASTLED_INTERNAL is only here to avoid warning messages.
#define FASTLED_INTERNAL 1
#ifndef STRUCTS_H
#define STRUCTS_H
#include <FastLED.h>

#define DEBUG 1
#define MOTION 0
#define USEMIXER 0
#define MAX_DIMENSION 16
#define AGRESSIVE 1

#define HEARTBEAT_PIN 13

// #define ALU_TUBE
#define CARDBOARD_TUBE
// #define TEST_BOARD

#ifdef ALU_TUBE
#define DATA_PIN 8         // test board, burning man box

// How many leds in your strip?
#define NUM_LEDS 288  // aluminum tube

// Button pins for up and down
#define PIN_UP 9
#define PIN_DOWN 11
#define PIN_EFFECT 6 // burning man box
#endif

#ifdef CARDBOARD_TUBE
#define DATA_PIN 8         // test board, burning man box

// How many leds in your strip?
#define NUM_LEDS 296  // cardboard tube

// Button pins for up and down
#define PIN_UP 9
#define PIN_DOWN 11
#define PIN_EFFECT 6
#endif

#ifdef TEST_BOARD
#define DATA_PIN 8
#define CONFIG_DATA_PIN 7

// How many leds in your strip?
#define NUM_LEDS 64
#define CONFIG_NUM_LEDS 8

// Button pins for up and down
#define PIN_UP 9
#define PIN_DOWN 11
#define PIN_EFFECT 16
#endif

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
