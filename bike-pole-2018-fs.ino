#include <FastLED.h>
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

// #ifdef MOTION
#include "motion.h"
// #endif

#define DATA_PIN 7

// Define the array of leds
// buffer 0 and 1 are for mainaining the
CRGB leds[3][NUM_LEDS];
#define mixedBuffer 2

Effect* effects[] = {
	new Noise(),
	new Flash(CRGB::Red),
	new Bounce(20, 220),
	new Sparkles(80, 5, true),
	new Pile(),
	// new Roller(CRGB::White, CRGB::White, 2),
};

const byte numEffects = (sizeof(effects) / sizeof(effects[0]));

Button briUpButton = Button(PIN_UP);
Button briDwButton = Button(PIN_DOWN);
Button effectButton = Button(PIN_EFFECT);
Controller controller = Controller(leds[0], leds[1]);
Palmixer palmixer = Palmixer((int)kNumPalettes,
		palettes.palettes, palettes.nextPalette, palettes.currentPalette,
		palettes.finalPalette);
Mixer mixer = Mixer(leds[0], leds[1], leds[2]);

// Global Brightness
uint8_t brightnessMap[] = { 16, 32, 64, 128, 255 };
const byte brightnessCount = (sizeof(brightnessMap) / sizeof(brightnessMap[0]));
uint8_t briLevel = 1;
uint8_t currentEffect = 0;

// Timers
// int timeTillPrint = 1000; // Print diagnostics once per second
// Initial timers
const int timeTilPrint = 10;
const int timeTilAnimate = 10;
const int timeTilRender = 16; // 60Hz rendering
const int timeTilOrientation = 16; // Let' stry 60hz for motion updates as well
const int timeTilPalChange = 10000000; // Let' stry 60hz for motion updates as well

int timeLeftTillPrint = timeTilPrint;
int timeLeftTilAnimate = timeTilAnimate;
int timeLeftTilRender = timeTilRender;
int timeLeftTilOrientation = timeTilOrientation;
int timeLeftTilPalChange = 0; // Force a palette change.

unsigned long lastPrint = 0;
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
bool demoMode = false;

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
  set_max_power_in_volts_and_milliamps(5, 2000);

	// InitMotion();
	GenerateGlobalPalettes();

	controller.Initialize();
	if (USEMIXER) {
		controller.SetBaseEffect(effects[0]);
		controller.SetLayerEffect(effects[3]);
  } else {
		controller.SetEffect(effects[currentEffect]);
		controller.SetBuffer(leds[2]);
	}
	effects[currentEffect]->Initialize();
}

void loop() {
	// Serial.println("Looping...");

  UpdateTimers();
	UpdatePalette();
	palmixer.Animate(deltaMicros);
	controller.Animate(deltaMicros);

  // Right now all the motion code is not working correctly.
	// UpdateMotion(micros());
	// getOrientation(&roll,&pitch,&heading,&x,&y,&z);
	// CheckBumps();

	CheckBrightness();
	CheckEffect();
	WaitForNextEffect();

	if (timeLeftTillPrint <= 0)
	{
		timeLeftTillPrint = timeTilPrint;
		// Printer();
	}
	// Render all active buffers and mixdown, then show with power limits applied
	if (timeLeftTilRender <= 0)
	{
		renderCount++;
		timeLeftTilRender = timeTilRender;
		controller.Render(palettes.finalPalette);
		mixer.Mix();
		FastLED.show();
  }
}

void UpdateTimers() {
	unsigned long currentMicros = micros();
	unsigned long currentMillis = millis();

  deltaMicros = currentMicros - lastMicros;
  lastMicros = currentMicros;

  deltaMillis = currentMillis - lastMillis;
  lastMillis = currentMillis;

  // Decrement our timers
  timeLeftTillPrint -= deltaMillis;
  timeLeftTilRender -= deltaMillis;
	timeLeftTilAnimate -= deltaMillis;
  timeLeftTilOrientation -= deltaMillis;
}

// move this code to palmixer.UpdatePalette() or palmixer.Animate()
void UpdatePalette() {
	// Periodically change the palette
	timeLeftTilPalChange -= deltaMicros;
	if (timeLeftTilPalChange <= 0)
	{
		// For now change all palettes
		// if (DEBUG) Serial.printf("Changing palettes...\n");
		int newpal = random(0, kNumPalettes);
		// one second fade to next palette
		palmixer.SetNewPalette(0, newpal, 4.0f);
		newpal = random(0, kNumPalettes);
		// one second fade to next palette
		palmixer.SetNewPalette(1, newpal, 4.0f);
		newpal = random(0, kNumPalettes);
		// one second fade to next palette
		palmixer.SetNewPalette(2, newpal, 4.0f);

		timeLeftTilPalChange = timeTilPalChange;
	}
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
	if (DEBUG) Serial.print("Changing effect to ");
	if (DEBUG) Serial.println(effects[currentEffect]->Identify());
	controller.Reset();
	// Override mixer and directly output the effect into mixed buffer.
	if (!USEMIXER) effects[currentEffect]->SetBuffer(leds[2]);
}

void CheckEffect() {
	int8_t button = effectButton.Read();
	if (button == 1) {
		if (DEBUG) Serial.println("button change pressed");
		waitingForEffectToEnd = true;
	}
}

void CheckBrightness() {
	int8_t buttonUp = briUpButton.Read();
	int8_t buttonDw = briDwButton.Read();
	if (buttonUp == 1) {
		if (DEBUG) Serial.println("button up pressed");
		if (DEBUG && briLevel < 4) Serial.println("Bri up");
		if (briLevel < 4) briLevel += 1;
	}
	if (buttonDw == 1) {
		if (DEBUG) Serial.println("button down pressed");
		if (DEBUG && briLevel > 0) Serial.println("Bri down");
		if (briLevel > 0) briLevel -= 1;
	}
	FastLED.setBrightness(brightnessMap[briLevel]);
}

void DebugPrint(String message) {
	if (DEBUG) {
		Serial.println(message);
	}
}

void Printer() {
	if (DEBUG) {
		Serial.print("Orientation (h, p, r): ");
	  Serial.print(heading);
	  Serial.print(" ");
	  Serial.print(pitch);
	  Serial.print(" ");
  	Serial.println(roll);
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
