#ifndef POC_SINGLETUBE_SRC_DISPLAYCONTROL_H_
#define POC_SINGLETUBE_SRC_DISPLAYCONTROL_H_

#include <FramingControl.h>
#include <Hv57708.hpp>

#define TUBE_SIZE 4
#define MAPPING_SIZE (TUBE_SIZE * 10)

using namespace std;

class DisplayControl {
 private:
  Hv57708 &device;
  uint8_t mapping[MAPPING_SIZE];

  // TODO: currently, number of tube is fixed
  FramingControl controls[TUBE_SIZE];
  int numbers[TUBE_SIZE] = { 0 };

 public:
  explicit DisplayControl(Hv57708 &device, uint8_t mapping[MAPPING_SIZE]) : device(device) {
    // mapping
    for (int i = 0; i < MAPPING_SIZE; ++i) {
      this->mapping[i] = mapping[i];
    }

    // control
    int buffer[10] = {0};
    buffer[0] = 1;

    for (auto &control : controls) {
      control.setNumbers(buffer);
    }
  }

  void setNumbers(const int input[TUBE_SIZE]) {
    for (int i = 0; i < TUBE_SIZE; ++i) {
      numbers[i] = input[i];
    }
  }

  void update() {
    uint32_t data_1 = 0;
    uint32_t data_2 = 0;

    // update frame of FramingControl
    int buffer[10];
    for (int control_index = 0; control_index < TUBE_SIZE; ++control_index) {
      auto &control = controls[control_index];
      control.update(buffer);

      for (int number = 0; number < 10; ++number) {
        setData(data_1, data_2, control_index, number, buffer[number]);
      }
    }

    // assumption:
    // all FramingControl are in-sync
    // i.e. all control will end simultaneously
    if (controls[0].isEnded()) {
      updateNumbers();
    }

    // update to device
    device.set(data_1, data_2);
  }

 private:
  void updateNumbers() {
    int buffer[10] = {0};

    for (int i = 0; i < TUBE_SIZE; ++i) {
      const int number = numbers[i];
      buffer[number] = 1;

      controls[i].setNumbers(buffer);

      buffer[number] = 0;
    }
  }

  void setData(uint32_t &data_1, uint32_t &data_2, int control_index, int number, int value) {
    if (value == 0) return;

    // get mapped port on HV57708
    // output ports in HV57708 is 1 indexed, and
    // here 0 in the mapping means "ignored / no mapping"
    const int i = control_index * 10 + number;
    int mapped_i = mapping[i];
    if (mapped_i == 0) return;

    mapped_i--;

    // set to data
    if (mapped_i < 32) {
      data_1 |= 1 << (32 - mapped_i - 1);

    } else {
      mapped_i -= 32;
      data_2 |= 1 << (32 - mapped_i - 1);
    }
  }
};

#endif //POC_SINGLETUBE_SRC_DISPLAYCONTROL_H_
