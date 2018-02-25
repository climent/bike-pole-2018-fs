// How many leds in your strip?
#define NUM_LEDS 210
// #define DATA_PIN 7

// Button pins for up and down
#define BTN_UP 9
#define BTN_DW 11

#ifndef STRUCTS_H
#define STRUCTS_H
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
