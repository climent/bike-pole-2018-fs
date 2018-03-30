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

// Define the array of leds:
// There 2 sets of 3 buffers. For each set:
//  * buffer 0 and 1 are for base and layer effects
//    * effect->Animate animates the effects
//    * effect->Render renders the effect in the buffers
//  * buffer 2 is a buffer for layer composition
//    * mixer->Mix renders the layering
//  * finally, outputBuffer is used to transition from one effect to another
//    * selector->ChangeEffect selects a new effect combination
//    *
CRGB leds[3][NUM_LEDS];
CRGB currentBuffers[3][NUM_LEDS];
CRGB nextBuffers[3][NUM_LEDS];
CRGB outputBuffer[NUM_LEDS];
#define mixedBuffer 2

Effect* null = new Null();
// Effect* null;
Effect* noise = new Noise();
Effect* flash =	new Flash(CRGB::Red);
Effect* bounce = new Bounce(20, 220);
Effect* sparkles = new Sparkles(80, 5, true);
Effect* pile = new Pile();
Effect* pools = new Pools();
Effect* modchase = new Modchase();

Effect* effects[] = {
	// null,
	modchase,
	pools,
	noise,
	// flash,
	// bounce,
	// sparkles,
	// pile,

	// new Roller(CRGB::White, CRGB::White, 2),
};

const byte numEffects = (sizeof(effects) / sizeof(effects[0]));

Button briUpButton = Button(PIN_UP);
Button briDwButton = Button(PIN_DOWN);
Button effectButton = Button(PIN_EFFECT);

// Controller controller = Controller(leds[0], leds[1]);
Controller controller = Controller(currentBuffers[0], currentBuffers[1],
		nextBuffers[0], nextBuffers[1]);

Palmixer palmixer = Palmixer(
		palettes.palettes, palettes.nextPalette, palettes.currentPalette,
	  palettes.finalPalette);
// Mixer mixer = Mixer(leds[0][0], leds[0][1], outputBuffer);

Mixer mixer = Mixer(outputBuffer);

// Global Brightness
uint8_t brightnessMap[] = { 16, 32, 64, 128, 255 };
const byte brightnessCount = (sizeof(brightnessMap) / sizeof(brightnessMap[0]));
uint8_t briLevel = 1;
uint8_t currentEffect = 0;

// Timers
// int timeTillPrint = 1000; // Print diagnostics once per second
// Initial timers
const int timeTilPrint = 10000;
const int timeTilAnimate = 10;
const int timeTilRender = 16; // 60Hz rendering
const int timeTilOrientation = 16; // Let' stry 60hz for motion updates as well
const int timeTilPalChange = 10000000; // Let' stry 60hz for motion updates as well

int timeLeftTillPrint = timeTilPrint;
int timeLeftTilAnimate = timeTilAnimate;
int timeLeftTilOrientation = timeTilOrientation;
int timeLeftTilRender = timeTilRender;
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
		outputBuffer, NUM_LEDS).setCorrection(TypicalLEDStrip);;
	FastLED.setDither(0);

	// Limit to 2 amps to begin with
  set_max_power_in_volts_and_milliamps(5, 2000);

	// InitMotion();
	GenerateGlobalPalettes();

	controller.Initialize();

	if (USEMIXER) {
		controller.SetBaseEffect(effects[0]);
		controller.SetLayerEffect(effects[1]);
		controller.SetOutputBuffer(outputBuffer);
  } else {
		controller.SetEffect(effects[currentEffect]);
		controller.SetBuffer(outputBuffer);
	}

	controller.SetTimer(timeTilRender);
	effects[currentEffect]->Initialize();

  // noise effect uses a palette to render colors
  noise->SetPaleteIndex(0);
	noise->SetPalette(palettes.finalPalette);
	pools->SetPaleteIndex(0);
	pools->SetPalette(palettes.finalPalette);
	modchase->SetPalette(palettes.finalPalette);

	palmixer.SetTimer(timeTilPalChange);
}

void loop() {
	// Serial.print("Looping...");
	framesCount++;

  UpdateTimers();

	palmixer.UpdatePalettes(deltaMicros);
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
		Printer();
	}

	// Render all active buffers and mixdown
	if (controller.Render(deltaMillis)) {
		renderCount++;
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
	controller.SetNextBaseEffect(effects[currentEffect]);
	if (DEBUG) {
		Serial.print("> Changing effects to: ");
	  Serial.print("[");
		Serial.print(controller.GetBaseEffect());
		Serial.print("] [");
		Serial.print(controller.GetLayerEffect());
		Serial.println("]");
	}
	controller.Reset();
	// Override mixer and directly output the effect into output buffer.
	if (!USEMIXER) effects[currentEffect]->SetBuffer(outputBuffer);
}

void CheckEffect() {
	if (effectButton.Read()) {
		if (DEBUG) Serial.println("  button change pressed");
		waitingForEffectToEnd = true;
	}
}

void CheckBrightness() {
	if (briUpButton.Read()) {
		if (DEBUG) Serial.println("  button up pressed");
		if (DEBUG && briLevel < 4) Serial.println("> Bri up");
		if (briLevel < 4) briLevel += 1;
	}
	if (briDwButton.Read()) {
		if (DEBUG) Serial.println("  button down pressed");
		if (DEBUG && briLevel > 0) Serial.println("> Bri down");
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
	Serial.printf("> frames: run [%d] render [%d]\n", framesCount, renderCount);
	framesCount = 0;
	renderCount = 0;
	Serial.print("> running effects: ");
	Serial.print("[");
	Serial.print(controller.GetBaseEffect());
	Serial.print("] [");
	Serial.print(controller.GetLayerEffect());
	Serial.println("]");

	if (DEBUG) {
		// Serial.print("Orientation (h, p, r): ");
	  // Serial.print(heading);
	  // Serial.print(" ");
	  // Serial.print(pitch);
	  // Serial.print(" ");
  	// Serial.println(roll);
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
