#ifndef POC_MV57708_SRC_HV57708_HPP_
#define POC_MV57708_SRC_HV57708_HPP_

// https://github.com/blanboom/NixieClock/blob/master/Firmware/HardwareDrivers/HV57708.c

#include <Arduino.h>

class Hv57708 {

 public:
  Hv57708(
      int din1_pin,
      int din2_pin,
      int din3_pin,
      int din4_pin,
      int pol_pin,
      int clk_pin,
      int le_pin) :
      din1_pin(din1_pin),
      din2_pin(din2_pin),
      din3_pin(din3_pin),
      din4_pin(din4_pin),
      pol_pin(pol_pin),
      clk_pin(clk_pin),
      le_pin(le_pin) {

    // set pin mode
    pinMode(pol_pin, OUTPUT);
    pinMode(clk_pin, OUTPUT);
    pinMode(le_pin, OUTPUT);
    pinMode(din1_pin, OUTPUT);
    pinMode(din2_pin, OUTPUT);
    pinMode(din3_pin, OUTPUT);
    pinMode(din4_pin, OUTPUT);

    // set initial state
    digitalWrite(pol_pin, LOW);
    digitalWrite(clk_pin, LOW);
    digitalWrite(le_pin, LOW);

    digitalWrite(din1_pin, LOW);
    digitalWrite(din2_pin, LOW);
    digitalWrite(din3_pin, LOW);
    digitalWrite(din4_pin, LOW);
  }

  void set(uint32_t data_1, uint32_t data_2) {
    sendData(data_1, data_2);
    outputData();
  }

 private:
  void sendData(uint32_t data_1, uint32_t data_2) const {
    uint8_t i;
    uint32_t tmp;

    tmp = data_2;
    for (i = 0; i < 8; i++) {
      digitalWrite(din4_pin, tmp & 1);
      tmp >>= 1;
      digitalWrite(din3_pin, tmp & 1);
      tmp >>= 1;
      digitalWrite(din2_pin, tmp & 1);
      tmp >>= 1;
      digitalWrite(din1_pin, tmp & 1);
      tmp >>= 1;

      delayMicroseconds(10);
      digitalWrite(clk_pin, HIGH);

      delayMicroseconds(10);
      digitalWrite(clk_pin, LOW);
    }

    tmp = data_1;
    for (i = 0; i < 8; i++) {
      digitalWrite(din4_pin, tmp & 1);
      tmp >>= 1;
      digitalWrite(din3_pin, tmp & 1);
      tmp >>= 1;
      digitalWrite(din2_pin, tmp & 1);
      tmp >>= 1;
      digitalWrite(din1_pin, tmp & 1);
      tmp >>= 1;

      delayMicroseconds(10);
      digitalWrite(clk_pin, HIGH);

      delayMicroseconds(10);
      digitalWrite(clk_pin, LOW);
    }
  }

 public:
  void outputData() const {
    delayMicroseconds(10);
    digitalWrite(le_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(le_pin, LOW);
  }

 private:
  const int din1_pin;
  const int din2_pin;
  const int din3_pin;
  const int din4_pin;
  const int pol_pin;
  const int clk_pin;
  const int le_pin;
};

#endif //POC_MV57708_SRC_HV57708_HPP_
