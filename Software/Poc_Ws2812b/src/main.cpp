#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 4
#define DATA_PIN 15
CRGB leds[NUM_LEDS];

void rgbFlashing() {
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CRGB::Green;
    FastLED.show();
    delay(500);

    leds[i] = CRGB::Black;
    FastLED.show();
    delay(500);
  }

  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(500);

    leds[i] = CRGB::Black;
    FastLED.show();
    delay(500);
  }

  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CRGB::Blue;
    FastLED.show();
    delay(500);

    leds[i] = CRGB::Black;
    FastLED.show();
    delay(500);
  }
}

void hueFlashing() {
  uint8_t hue = 0;
  for (int i = 0; i < 255; ++i) {
    for (auto & led : leds) {
      led = CHSV(hue, 255, 255);
    }
    hue++;

    FastLED.show();
    delay(10);
  }
}

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
  FastLED.setBrightness(32);
}

void loop() {
  hueFlashing();
}