#include <Arduino.h>
#include <FastLED.h>
#include <Hv57708.hpp>
#include <FramingControl.h>
#include <DisplayControl.h>
#include <Config.h>
#include <Wire.h>
#include <DS3231.h>

#define NUM_LEDS 4
#define DATA_PIN 15

CRGB leds[NUM_LEDS];
uint8_t hue = 170;
uint8_t hue_direction = 1;
uint32_t tick_light = 0;

Hv57708 hv57708(
    HV57708_D1,
    HV57708_D2,
    HV57708_D3,
    HV57708_D4,
    HV57708_POL,
    HV57708_CLK,
    HV57708_LE);
uint8_t mapping[40] = {
    10, 9, 8, 7, 6, 5, 4, 3, 2, 1  // tube 1
};
DisplayControl display_control(hv57708, mapping);
int tick_display = 0;

RTClib rtc;
DS3231 ds3231;

uint32_t tick_led = 0;
int led_state = LOW;

void setCurrentTime() {
  constexpr time_t timestamp = 1687585135;
  ds3231.setEpoch(timestamp, false);
  ds3231.setClockMode(false);
}

void setup() {
  // debug
  Serial.begin(57600);

  // light
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
  FastLED.setBrightness(32);
  for (auto &led : leds) {
    led = CHSV(hue, 255, 255);
  }
  FastLED.show();

  // clock
  Wire.begin(22, 23);
  setCurrentTime();

  // led
  pinMode(2, OUTPUT);
}

void loop() {
  // display
  tick_display++;
  display_control.update();

  if (tick_display >= 250) {
    tick_display = 0;

    DateTime now = rtc.now();
    int numbers[4];
    numbers[0] = now.second() % 10;
    numbers[1] = now.second() / 10;
    numbers[2] = now.minute() % 10;
    numbers[3] = now.minute() / 10;

    // Serial.printf("%i%i:%i%i\n", numbers[3], numbers[2], numbers[1], numbers[0]);
    display_control.setNumbers(numbers);
  }

  // light
  tick_light++;
  if (tick_light >= 100) {
    tick_light = 0;

    for (auto &led : leds) {
      led = CHSV(hue, 255, 255);
    }
    FastLED.show();

    hue += hue_direction;
    if (hue > 220) hue_direction = -1;
    else if (hue < 160) hue_direction = 1;
  }

  // led
  tick_led++;
  if (tick_led >= 1000) {
    tick_led = 0;

    led_state = led_state == LOW ? HIGH : LOW;
    digitalWrite(2, led_state);
  }

  delay(1);
}