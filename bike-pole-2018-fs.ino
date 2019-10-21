// This FASTLED_INTERNAL is only here to avoid warning messages.
#define FASTLED_INTERNAL 1
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

#define DATA_PIN 8
#define HEARTBEAT_PIN 13
#define AGRESSIVE 1

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
Effect* noise = new Noise();
Effect* flash =	new Flash(CRGB::Red);
Effect* bounce = new Bounce(20, 220);
Effect* sparkles = new Sparkles(80, 5, true);
Effect* pile = new Pile();
Effect* pools = new Pools();
Effect* modchase = new Modchase();
Effect* twinkles = new Twinkles(80, 5);
Effect* paltest = new PalTest();
// Effect* test0 = new Test(20, CRGB::Black);
// Effect* test1 = new Test(10, CRGB::Red);
// Effect* test2 = new Test(20, CRGB::Blue);
// Effect* roller = new Roller(CRGB::White, CRGB::White, 2),

Effect* effects[] = {
	paltest,
	// null,
	// test0,
  // test1,
	// test2,
	// modchase,
	pools,
	noise,
	// flash,
	// bounce,
	// twinkles,
	sparkles,
	// pile,
};

const byte numEffects = (sizeof(effects) / sizeof(effects[0]));

//Button briUpButton = Button(PIN_UP);
//Button briDwButton = Button(PIN_DOWN);
//Button effectButton = Button(PIN_EFFECT);
Button eventButton = Button(PIN_EFFECT);

// Controller controller = Controller(leds[0], leds[1]);
Controller controller = Controller(
		currentBuffers[0],
		currentBuffers[1],
		nextBuffers[0],
		nextBuffers[1]);

Palmixer palmixer = Palmixer(
		palettes.palettes,
		palettes.currentPalette,
		palettes.nextPalette,
	  palettes.finalPalette);

Mixer mixer = Mixer(outputBuffer);

// Global Brightness
int brightnessMap[] = { 8, 16, 32, 64, 128, 255 };
const byte brightnessCount = (sizeof(brightnessMap) / sizeof(brightnessMap[0]));
int briLevel = 1;

int currentEffect = 0;
int nextEffect = 0;
int currentBaseEffect = 0;
int currentLayerEffect = 0;
int nextBaseEffect = 0;
int nextLayerEffect = 0;

// Timers
const long timeTilPrint = 1000;
const long timeTilAnimate = 10;
const long timeTilRender = 16; // 60Hz rendering
const long timeTilOrientation = 16; // Let' stry 60hz for motion updates as well
const long timeTilPalChange = 1000000;

// Decide which one to use.
const float transitionTimer = 0.2f;
float timeForPaletteTransition = 0.2f;  // time 

long timeLeftTillPrint = timeTilPrint;
long timeLeftTilAnimate = timeTilAnimate;
long timeLeftTilOrientation = timeTilOrientation;
long timeLeftTilRender = timeTilRender;
long timeLeftTilPalChange = 0; // Force a palette change.


unsigned long lastPrint = 0;
unsigned long lastMillis = 0;
unsigned long lastMicros = 0;
unsigned long deltaMillis = 0;
unsigned long deltaMicros = 0;
unsigned long currentMicros;
unsigned long currentMillis;

// Counters
int framesCount = 0;
int renderCount = 0;

// Global aggressive var. If set, changing effects does not wait 
// for completion
bool aggressive = false;
bool demoMode = false;
bool outputSingleString = false;

void setup() {
	Serial.begin(115200);
	delay(1000); // Let the serial monitor come up
	Serial.print("Initializing... ");

	int val = analogRead(0); // read random value;
  randomSeed(val);

  Serial.printf("Random seed is: %d\n", val);
	Serial.printf("Number of Effects: %d\n", numEffects);

	// FastLED.addLeds<WS2812B, DATA_PIN, GRB>(
	FastLED.addLeds<NEOPIXEL, DATA_PIN>(
		outputBuffer, NUM_LEDS).setCorrection(TypicalLEDStrip);;
	FastLED.setDither(0);

	// Limit to 2 amps to begin with
  set_max_power_in_volts_and_milliamps(5, 2000);

	// InitMotion();
	GenerateGlobalPalettes();

	palmixer.Initialize();
	palmixer.SetRandomPalettes(false);
	palmixer.SetTimer(timeTilPalChange);
	palmixer.SetTransitionTimer(transitionTimer);

	controller.Initialize();
	controller.SetTimer(timeTilRender);
	controller.SetPalette(palettes.finalPalette);

	if (USEMIXER) {
		controller.SetBaseEffect(effects[0]);
		controller.SetLayerEffect(effects[1]);
		controller.SetOutputBuffer(outputBuffer);
  } else {
		controller.SetEffect(effects[currentEffect]);
		controller.SetOutputBuffer(outputBuffer);
	}

	effects[currentEffect]->Initialize();

  // noise effect uses a palette to render colors
	// noise->SetPalette(palettes.finalPalette);
	// pools->SetPalette(palettes.finalPalette);
	// paltest->SetPalette(palettes.finalPalette);
	// modchase->SetPalette(palettes.finalPalette);

	pinMode(HEARTBEAT_PIN, OUTPUT);

	Serial.println("Setup done...");
}

bool black = true;
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

  CheckButtonEvent();

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
	if (lastMillis > currentMillis){
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

	if (deltaMillis > currentMillis){
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
	currentEffect += 1;
	if (currentEffect == numEffects) currentEffect = 0;
	controller.InitiateTransition(effects[currentEffect], aggressive);
	if (DEBUG) {
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
	controller.Reset();
	// Override mixer and directly output the effect into output buffer.
	// if (!USEMIXER) effects[currentEffect]->SetBuffer(outputBuffer);
}

void CheckButtonEvent() {
	int event	= eventButton.ReadEvent();
	switch(event) {
		case 1:
			if (DEBUG) Serial.println("  button event 1");
			NextEffect();
			break;
		case 2:
			if (DEBUG && briLevel > 0)
			  Serial.print("> Bri down: ");
			if (briLevel > 0) {
				briLevel -= 1;
				if (DEBUG) Serial.printf("%d\n", brightnessMap[briLevel]);
			}
			break;
		case 3:
			if (DEBUG && briLevel < brightnessCount - 1)
				Serial.print("> Bri up: ");
			if (briLevel < brightnessCount - 1) {
				briLevel += 1;
			if (DEBUG) Serial.printf("%d\n", brightnessMap[briLevel]);
			}
			break;
	}
	FastLED.setBrightness(brightnessMap[briLevel]);
}

// void CheckEffect() {
// 	if (eventButton.Read()) {
// 		if (DEBUG) Serial.println("  button change pressed");
// 		NextEffect();
// 	}
// }
// void CheckBrightness() {
// 	if (briUpButton.Read()) {
// 		if (DEBUG) Serial.println("  button up pressed");
// 		if (DEBUG && briLevel < brightnessCount - 1)
// 			Serial.print("> Bri up: ");
// 		if (briLevel < brightnessCount - 1) {
// 			briLevel += 1;
// 			if (DEBUG) Serial.printf("%d\n", brightnessMap[briLevel]);
// 		}
// 	}
// 	if (briDwButton.Read()) {
// 		if (DEBUG) Serial.println("  button down pressed");
// 		if (DEBUG && briLevel > 0)
// 		  Serial.print("> Bri down: ");
// 		if (briLevel > 0) {
// 			briLevel -= 1;
// 			if (DEBUG) Serial.printf("%d\n", brightnessMap[briLevel]);
// 		}
// 	}
// 	FastLED.setBrightness(brightnessMap[briLevel]);
// }

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
	Serial.print("[b: ");
	Serial.print(controller.GetBaseEffect());
	Serial.print("] [l: ");
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
		gotBumped = false;
		maxAx = 0.0;
		maxAy = 0.0;
		maxAz = 0.0;
		minAx = 0.0;
		minAy = 0.0;
		minAz = 0.0;
	}
}
