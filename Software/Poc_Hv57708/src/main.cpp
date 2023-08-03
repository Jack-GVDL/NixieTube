#include <Arduino.h>
#include <Hv57708.hpp>
#include <FramingControl.h>

Hv57708 hv57708(21, 19, 18, 5, 17, 16, 4);
FramingControl framingControl(hv57708);
int cur = 0;
int state = LOW;
int counter = 0;

void setup() {
  pinMode(2, OUTPUT);

  vector<int> numbers(10, 0);
  numbers[cur] = 1;
  framingControl.setNumbers(numbers);
  cur++;
}

void loop() {
  framingControl.update();

  if (framingControl.isEnded()) {
    vector<int> numbers(10, 0);
    numbers[cur / 2] = 1;
    framingControl.setNumbers(numbers);

    cur = (cur + 1) % 20;
  }

  delay(1);

  counter++;
  if (counter > 1000) {
    counter = 0;

    state = state == LOW ? HIGH : LOW;
    digitalWrite(2, state);
  }
}