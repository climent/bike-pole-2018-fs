// This FASTLED_INTERNAL is only here to avoid warning messages.
#define FASTLED_INTERNAL 1

#include <FastLED.h>
#include <EEPROM.h>

#include "palette_data.h"
#include "palettes.h"
#include "includes.h"
#include "effect.h"
#include "controller.h"
#include "button.h"
#include "color_utils.h"
#include "palmixer.h"
#include "mixer.h"
#include "selector.h"
#include "eeprom.h"

// #ifdef MOTION
// #include "motion.h"
// #endif

#define DATA_PIN 8

// Define the array of leds
// buffer 0 and 1 are for mainaining the
// CRGB leds[3][NUM_LEDS];
// CRGBArray<NUM_LEDS> leds[3];

CRGBArray<NUM_LEDS> leds[3];
CRGBArray<NUM_LEDS> currentBuffers[3];
CRGBArray<NUM_LEDS> nextBuffers[3];
CRGBArray<NUM_LEDS> outputBuffer;
// CRGB outputBuffer[NUM_LEDS];

#define mixedBuffer 2

Effect* null = new Null();
Effect* noise = new Noise();
Effect* flash =  new Flash(CRGB::Red);
Effect* bounce = new Bounce(2, 245);
Effect* sparkles = new Sparkles(80, 5, true);
Effect* pile = new Pile();
Effect* plasma = new Plasma();
// Effect* modchase = new Modchase();
Effect* twinkles = new Twinkles(80, 5);
Effect* paltest = new PalTest();
// Effect* test0 = new Test(20, CRGB::Black);
// Effect* test1 = new Test(10, CRGB::Red);
// Effect* test2 = new Test(20, CRGB::Blue);
// Effect* roller = new Roller(CRGB::White, CRGB::White, 2),


Effect* effects[] = {
  // paltest,
  // null,
  // test0,
  // test1,
  // test2,
  plasma,
  pile,
  noise,
  flash,
  bounce,
  sparkles,
  // modchase,
  twinkles,
};

const byte numEffects = (sizeof(effects) / sizeof(effects[0]));

Button mainButton  = Button(PIN_EFFECT);
// Button onoffButton = Button(PIN_ON_OFF);

// Controller controller = Controller(leds[0], leds[1]);
Controller controller = Controller(
    currentBuffers[0],
    currentBuffers[1],
    nextBuffers[0],
    nextBuffers[1]);

Palmixer palmixer = Palmixer(
    palettes.palettes,
    palettes.nextPalette,
    palettes.currentPalette,
    palettes.finalPalette);

// Mixer      mixer      = Mixer(leds[0], leds[1], leds[2]);
Mixer mixer = Mixer(outputBuffer);

// Global Brightness
const uint8_t brightnessMap[] = { 0, 8, 16, 32, 64, 128, 255 };
const byte    brightnessCount = (sizeof(brightnessMap) / sizeof(brightnessMap[0]));
uint8_t       briLevel        = 2;

uint8_t currentEffect      = 0;
uint8_t nextEffect         = 0;
uint8_t currentBaseEffect  = 0;
uint8_t currentLayerEffect = 0;
uint8_t nextBaseEffect     = 0;
uint8_t nextLayerEffect    = 0;

const uint8_t limits[4][2] PROGMEM = {
  { 0, brightnessCount }, // 0
  { 0, numEffects },      // 1
  { 0, 1 },               // 2
  { 0, 1 },               // 3
};

// Global eeprom state
bool eepromOutdated = false;

// Timers
// int timeTillPrint = 1000; // Print diagnostics once per second
// Initial timers
const int timeTilPrint       = 10;
const int timeTilAnimate     = 10;
const int timeTilRender      = 16; // 60Hz rendering
const int timeTilOrientation = 16; // Let' stry 60hz for motion updates as well
const int timeTilPalChange   = 10000000; // Let' stry 60hz for motion updates as well

// Decide which one to use.
const float transitionTimer    = 2.0f;
float timeForPaletteTransition = 2.0f;  // time

int timeLeftTillPrint      = timeTilPrint;
int timeLeftTilAnimate     = timeTilAnimate;
int timeLeftTilRender      = timeTilRender;
int timeLeftTilOrientation = timeTilOrientation;
int timeLeftTilPalChange   = 0; // Force a palette change.

unsigned long lastPrint   = 0;
unsigned long lastMillis  = 0;
unsigned long lastMicros  = 0;
unsigned long deltaMillis = 0;
unsigned long deltaMicros = 0;
unsigned long currentMicros;
unsigned long currentMillis;

// Counters
int framesCount = 0;
int renderCount = 0;

// Global aggressive var. If set, changing effects does not wait for completion
bool aggressive = true;
bool demoMode = false;
bool waitingForEffectToEnd = false;
bool outputSingleString = false;

void setup() {
  Serial.begin(115200);
  delay(1000); // Let the serial monitor come up
  Serial.print("Initializing... ");
  Serial.println("");

  // EEPROM.begin(512);
  // Read EEPROM values
  uint8_t briLevelTemp = ReadEEPROM(0);
  if ((briLevelTemp >= limits[0][0]) || (briLevelTemp <= limits[0][1])) {
    briLevel = briLevelTemp;
  }
  int currentEffectTemp = ReadEEPROM(1);
  if ((currentEffectTemp >= limits[1][0]) || (currentEffect <= limits[1][1])) {
    currentEffect = currentEffectTemp;
  }
  nextEffect = currentEffect + 1;
  if (nextEffect == numEffects) nextEffect = 0;

  int val = analogRead(0); // read random value;
  randomSeed(val);

  Serial.printf("Random seed is: %d\n", val);
  Serial.printf("Number of Effects: %d\n", numEffects);

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(
    outputBuffer, NUM_LEDS).setCorrection(TypicalLEDStrip).setTemperature(DirectSunlight);
  FastLED.setDither(1);

  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.clear();
  FastLED.show();

  // Limit to 2 amps to begin with
  // set_max_power_in_volts_and_milliamps(5, 1000);

  // InitMotion();
  GenerateGlobalPalettes();

  palmixer.Initialize();
  palmixer.SetRandomPalettes(true);
  palmixer.SetTimer(timeTilPalChange);
  palmixer.SetTransitionTimer(transitionTimer);
  palmixer.SetNumPalettes(palettes.numPalettes);

  controller.Initialize();
  controller.SetTimer(timeTilRender);
  controller.SetPalette(palettes.finalPalette);

  if (USEMIXER) {
    // controller.SetBaseEffect(effects[currentEffect]);
    // controller.SetLayerEffect(effects[3]);
    controller.SetBaseEffect(effects[currentEffect]);
    controller.SetLayerEffect(effects[nextEffect]);
    controller.SetOutputBuffer(outputBuffer);
  } else {
    controller.SetEffect(effects[currentEffect]);
    // controller.SetBuffer(leds[2]);
    controller.SetOutputBuffer(outputBuffer);
  }
  effects[currentEffect]->Initialize();

  pinMode(HEARTBEAT_PIN, OUTPUT);
  Serial.println("Setup done...");
}

bool hearbeat = true;

void loop() {
  // Serial.println("Looping...");
  framesCount++;

  UpdateTimers();

  palmixer.Animate(deltaMicros);
  controller.Animate(deltaMicros);

  // Right now all the motion code is not working correctly.
  // UpdateMotion(micros());
  // getOrientation(&roll,&pitch,&heading,&x,&y,&z);
  // CheckBumps();

  CheckButtons();
  // CheckOnOff();

  if (timeLeftTillPrint <= 0) {
    timeLeftTillPrint = timeTilPrint;
  }

  // Render all active buffers and mixdown
  if (controller.Render(deltaMillis)) {
    renderCount++;
    FastLED.show();
  }

  EVERY_N_SECONDS(1) {
    hearbeat == true ? hearbeat = false : hearbeat = true;
  }
  hearbeat == true ? digitalWrite(HEARTBEAT_PIN, HIGH) : digitalWrite(HEARTBEAT_PIN, LOW);
}

void UpdateTimers() {
  currentMicros = micros();
  currentMillis = millis();

  deltaMicros = currentMicros - lastMicros;
  lastMicros = currentMicros;

  // EVERY_N_MILLISECONDS(5) {
    // if (controller.IsTransitionActive()){
  if ((DEBUG) && (lastMillis > currentMillis)) {
    Serial.print("deltaMillis: ");
    Serial.print(deltaMillis);
    Serial.print(" currentMillis: ");
    Serial.print(currentMillis);
    Serial.print(" lastMillis: ");
    Serial.println(lastMillis);
  }
    // }
  // }

  deltaMillis = currentMillis - lastMillis;
  lastMillis = currentMillis;

  if ((DEBUG) && (deltaMillis > currentMillis)) {
    Serial.print("deltaMillis: ");
    Serial.print(deltaMillis);
    Serial.print(" currentMillis: ");
    Serial.print(currentMillis);
    Serial.print(" lastMillis: ");
    Serial.println(lastMillis);
  }

  // Decrement our timers
  timeLeftTillPrint -= deltaMillis;
  timeLeftTilRender -= deltaMillis;
  timeLeftTilAnimate -= deltaMillis;
  timeLeftTilOrientation -= deltaMillis;
}

void NextEffect() {
  waitingForEffectToEnd = false;
  currentEffect += 1;
  if (currentEffect == numEffects) currentEffect = 0;
  if (USEMIXER) {
    controller.InitiateTransition(effects[currentEffect], aggressive);
  } else {
    controller.SetEffect(effects[currentEffect]);
    FastLED.clear();
     FastLED.show();
  }

  if (DEBUG) {
    if (aggressive) {
      Serial.print(F("Changing effect to index "));
      Serial.print(currentEffect);
      Serial.print(", ");
      Serial.println(effects[currentEffect]->Identify());
    } else {
      Serial.print("> Changing effects to: ");
      Serial.print("[");
      Serial.print(controller.GetNextBaseEffect());
      Serial.print("] [");
      Serial.print(controller.GetNextLayerEffect());
      Serial.println("]");
      Serial.print("deltaMillis: ");
      Serial.print(deltaMillis);
      Serial.print(" currentMillis: ");
      Serial.print(currentMillis);
      Serial.print(" lastMillis: ");
      Serial.println(lastMillis);
    }
  }

  controller.Reset();

  WriteEEPROM(1, currentEffect);
}

void CheckButtons() {
  int8_t button = mainButton.Read();
  if (button != 0) eepromOutdated = true;
  if (button == 1) {
    if (DEBUG) Serial.println("CheckButtons(): Change effect");
    NextEffect();
    // waitingForEffectToEnd = true;
  }
  if (button == 2) {
    // if (DEBUG) Serial.println("button up pressed");
    if (DEBUG && briLevel < 4) Serial.println("CheckButtons(): Brightness up");
    if (briLevel < limits[0][1]) briLevel += 1;
  }
  if (button == 3) {
    // if (DEBUG) Serial.println("button down pressed");
    if (DEBUG && briLevel > 0) Serial.println("CheckButtons(): Brightness down");
    if (briLevel > limits[0][0]) briLevel -= 1;
  }
  if ((button == 2) || (button == 3)) {
    WriteEEPROM(0, briLevel);
  }
  FastLED.setBrightness(brightnessMap[briLevel]);
}

void DebugPrint(String message) {
  if (DEBUG) {
    Serial.println(message);
  }
}

// void Printer() {
//   if (DEBUG) {
//     Serial.print("Orientation (h, p, r): ");
//     Serial.print(heading);
//     Serial.print(" ");
//     Serial.print(pitch);
//     Serial.print(" ");
//     Serial.println(roll);
//  }
// }

// void CheckBumps() {
//   if (gotBumped) {
//     waitingForEffectToEnd = true;
//     gotBumped = false;
//     maxAx = 0.0;
//     maxAy = 0.0;
//     maxAz = 0.0;
//     minAx = 0.0;
//     minAy = 0.0;
//     minAz = 0.0;
//   }
// }
