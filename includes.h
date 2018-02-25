// How many leds in your strip?
#define NUM_LEDS 210

// Use CLOCK_PIN for apa102 type of LED strips
#define DATA_PIN 7
//#define CLOCK_PIN 13

// Button pins for up and down
#define BTN_UP 9
#define BTN_DW 11

const int intLedPin = 13;

// Global Brightness
const uint8_t brightnessCount = 5;
uint8_t brightnessMap[brightnessCount] = { 16, 32, 64, 128, 255 };
uint8_t briLevel = 2;

uint8_t position = 0;

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
