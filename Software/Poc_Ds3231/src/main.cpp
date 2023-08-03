#include <Arduino.h>
#include <Wire.h>
#include <DS3231.h>

int led_state = LOW;
RTClib rtc;
DS3231 ds3231;

void showCurrentTime() {
  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  Serial.print(" since midnight 1/1/1970 = ");
  Serial.print(now.unixtime());
  Serial.print("s = ");
  Serial.print(now.unixtime() / 86400L);
  Serial.println("d");
}

void setCurrentTime() {
  constexpr time_t timestamp = 1687585135;
  ds3231.setEpoch(timestamp, false);
  ds3231.setClockMode(false);
}

void setup() {
  Serial.begin(57600);
  Wire.begin(22, 23);
  pinMode(2, OUTPUT);
  delay(500);

  Serial.println("Ready!");
  setCurrentTime();
}

void loop() {
  showCurrentTime();

  led_state = led_state == LOW ? HIGH : LOW;
  digitalWrite(2, led_state);
  delay(1000);
}