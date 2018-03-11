#include <FastLED.h>
#include "includes.h"
#include "effect.h"
#include "controller.h"
#include "buttons.h"
// #include "palettes.h"

// #ifdef MOTION
#include "motion.h"
// #endif

#define DATA_PIN 7

// Define the array of leds
// buffer 0 and 1 are for mainaining the
CRGB leds[3][NUM_LEDS];
#define mixedBuffer 2

Effect* effects[] = {
	// new Noise(),
	// new Flash(CRGB::Red),
	// new Bounce(20, 220),
	// new Sparkles(80, 5, true),
	new Pile(),
	new Noise(),
	// new Roller(CRGB::White, CRGB::White, 2),
};

const byte numEffects = (sizeof(effects) / sizeof(effects[0]));

Buttons briButtons = Buttons(PIN_UP, PIN_DOWN);
Buttons effectButtons = Buttons(PIN_EFFECT);
Controller controller = Controller(leds[0], leds[1]);

// Global Brightness
const uint8_t brightnessCount = 5;
uint8_t brightnessMap[brightnessCount] = { 16, 32, 64, 128, 255 };
uint8_t briLevel = 1;
uint8_t currentEffect = 0;

// Timers
// int timeTillPrint = 1000; // Print diagnostics once per second
// Initial timers
int timeTillAnimate = 10;
int timeTillRender = 16; // 60Hz rendering
int timeTillOrientation = 16; // Let' stry 60hz for motion updates as well

int timeLeftTillAnimate = 0;
int timeLeftTillRender = 0;
int timeLeftTillOrientation = 0;

unsigned long lastMillis = 0;
unsigned long lastMicros = 0;
unsigned long deltaMillis = 0;
unsigned long deltaMicros = 0;

// Counters
int framesCount = 0;
int renderCount = 0;

// Global aggressive var. If set, changing effects does not wait for completion
bool aggressive = true;
bool waitingForEffectToEnd = false;

void setup() {
	Serial.begin(115200);
	Serial.print("Resetting... ");

	int val = analogRead(0); // read random value;
  Serial.printf("Random seed is: %d\n", val);
  randomSeed(val);

	FastLED.addLeds<WS2812B, DATA_PIN, GRB>(
		leds[2], NUM_LEDS).setCorrection(TypicalLEDStrip);;
	FastLED.setDither(0);

	// Limit to 2 amps to begin with
  set_max_power_in_volts_and_milliamps(5, 1000);

	// InitMotion();
	// controller.Init();

	// GenerateGlobalPalettes();

	controller.SetBaseEffect(effects[currentEffect]);
	controller.SetBuffer(leds[2]);
	effects[currentEffect]->Initialize();
}

void loop() {
	// Serial.println("Looping...");

  UpdateTimers();
	controller.Animate(deltaMicros);
	// UpdateMotion(micros());
	// CheckBumps();
	CheckBrightness();
	CheckEffect();
	WaitForNextEffect();
	FastLED.show();
}

void UpdateTimers() {
	unsigned long currentMicros = micros();
	unsigned long currentMillis = millis();

  deltaMicros = currentMicros - lastMicros;
  lastMicros = currentMicros;

  deltaMillis = currentMillis - lastMillis;
  lastMillis = currentMillis;

  // Decrement our timers
  // timeLeftTillPrint -= deltamillis;
  timeLeftTillRender -= deltaMillis;
	timeLeftTillAnimate -= deltaMillis;
  timeLeftTillOrientation -= deltaMillis;
}

void WaitForNextEffect() {
	if (waitingForEffectToEnd) {
		if (!aggressive) {
			if (controller.CheckEnd()) {
				NextEffect();
			}
		} else {
			NextEffect();
		}
	}
}

void NextEffect() {
	waitingForEffectToEnd = false;
	currentEffect += 1;
	if (currentEffect == numEffects) currentEffect = 0;
	controller.SetEffect(effects[currentEffect]);
	Serial.print("Changing effect to ");
	Serial.println(effects[currentEffect]->Identify());
	controller.Reset();
	effects[currentEffect]->SetBuffer(leds[2]);
}

void CheckEffect() {
	int8_t button = effectButtons.Read();
	if (button == 1) {
		waitingForEffectToEnd = true;
	}
}

void CheckBrightness() {
	int8_t button = briButtons.Read();
	switch (button) {
	case 1:
		if (briLevel < 4) briLevel += 1;
		break;
	case -1:
		if (briLevel > 0) briLevel -= 1;
		break;
	}
	FastLED.setBrightness(brightnessMap[briLevel]);
}

void DebugPrint(String message) {
	if (DEBUG) {
		Serial.println(message);
	}
}

void CheckBumps() {
	if (gotBumped) {
		waitingForEffectToEnd = true;
		gotBumped = false;
		maxAx = 0.0;
		maxAy = 0.0;
		maxAz = 0.0;
		minAx = 0.0;
		minAy = 0.0;
		minAz = 0.0;
	}
}
