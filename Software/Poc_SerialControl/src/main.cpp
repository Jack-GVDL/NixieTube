#include <Arduino.h>
#include <SerialControl.hpp>
#include <iostream>

SerialControl serialControl;
int ledState = LOW;
int delayMs = 5000;

void onSerialAvailable();
void connectWifi(std::string &output, std::string &args);
void configDelay(std::string &output, std::string &args);

void setup() {
  serialControl.add("connect", connectWifi);
  serialControl.add("delay", configDelay);

  Serial.begin(115200);
  Serial.onReceive(onSerialAvailable);

  pinMode(2, OUTPUT);
}

void loop() {
  ledState = ledState == LOW ? HIGH : LOW;
  digitalWrite(2, ledState);

  delay(delayMs);
}

void onSerialAvailable() {
  while (Serial.available()) {
    auto s = Serial.readString();
    auto output = serialControl.push(s);
    Serial.print(output.c_str());
  }
}

void connectWifi(std::string &output, std::string &args) {
  output += "connect to (";
  output += args;
  output += ")";
}

void configDelay(std::string &output, std::string &args) {
  const int value = std::atoi(args.c_str());
  if (value < 100) {
    output += "input invalid";
    return;
  }

  delayMs = value;

  output += "new delay (";
  output += args;
  output += ")";
}
