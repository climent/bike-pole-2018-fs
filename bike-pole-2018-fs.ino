#include <FastLED.h>
#include "includes.h"
#include "effect.h"
#include "led_utils.h"
#include "buttons.h"

#define DATA_PIN 7

// Define the array of leds
// buffer 0 and 1 are for mainaining the
CRGB leds[3][NUM_LEDS];

Effect *effects[] = {
	new Flash(CRGB::Red),
	new Bounce(20, 220),
	new Sparkles(80, 5, true),
	new Pile(CRGB::White, CRGB::White, 10),
};

const byte numEffects = (sizeof(effects) / sizeof(effects[0]));

Buttons briButtons = Buttons(PIN_UP, PIN_DOWN);
Buttons effectButtons = Buttons(PIN_EFFECT);

// Global Brightness
const uint8_t brightnessCount = 5;
uint8_t brightnessMap[brightnessCount] = { 16, 32, 64, 128, 255 };
uint8_t briLevel = 2;
uint8_t currentEffect = 0;

// Timers

// Global aggressive var. If set, changing effects does not wait for completion
bool aggressive = false;
bool waitForNextEffect = false;

void setup() {
	Serial.begin(115200);
	Serial.println("resetting");
	FastLED.addLeds<WS2812B, DATA_PIN, GRB>(
		leds[2], NUM_LEDS).setCorrection(TypicalLEDStrip);;
	effects[currentEffect]->SetBuffer(leds[2]);
}

void loop() {
	effects[currentEffect]->Animate();
	CheckBrightness();
	CheckEffect();
	WaitForNextEffect();
	FastLED.show();
}

void WaitForNextEffect() {
	if (waitForNextEffect) {
    Serial.println("Waiting for effect to finish...");
		if (effects[currentEffect]->CheckEnd()) {
      NextEffect();
			waitForNextEffect = false;
		}
	}
}

void NextEffect() {
	currentEffect += 1;
	if (currentEffect == numEffects) currentEffect = 0;
	Serial.print("Changing effect to ");
	Serial.println(effects[currentEffect]->Identify());
  effects[currentEffect]->Reset();
  effects[currentEffect]->SetBuffer(leds[2]);
}

void CheckEffect() {
	int8_t button = effectButtons.Read();
	if (button == 1) {
		if (aggressive) {
      NextEffect();
		} else {
			waitForNextEffect = true;
		}
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
