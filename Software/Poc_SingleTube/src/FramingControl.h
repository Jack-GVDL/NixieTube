#ifndef POC_MV57708_SRC_FRAMINGCONTROL_H_
#define POC_MV57708_SRC_FRAMINGCONTROL_H_

#include <vector>

using namespace std;

class FramingControl {
 private:
  vector<vector<int>> frames;
  vector<int> numbers;

  int cur_step;
  int step_size;

  int cur_frame;
  int frame_size;

 public:
  explicit FramingControl() {
    this->cur_step = 0;
    this->step_size = 10;

    this->cur_frame = 0;
    this->frame_size = 50;

    this->frames = vector<vector<int>>(frame_size, vector<int>(step_size, 0));
    this->numbers = vector<int>(10, 0);
  }

  bool isEnded() const {
    return cur_frame == frame_size;
  }

  void restart() {
    this->cur_frame = 0;
    this->cur_step = 0;
  }

  void setNumbers(const int input[10]) {
    const int n = 10;

    for (int i = 0; i < n; ++i) {
      if (input[i] == numbers[i]) {
        if (numbers[i]) computeFrame(i, true);
        else computeFrame(i, false);
        continue;
      }

      if (input[i]) computeAppearing(i);
      else computeDisappearing(i);

      numbers[i] = input[i];
    }

    restart();
  }

  void update(int output[10]) {
    if (isEnded()) {
      return;
    }

    // output
    auto &frame = frames[cur_frame];

    for (int i = 0; i < 10; ++i) {
      const bool enabled = cur_step < frame[i];
      output[i] = enabled ? 1 : 0;
    }

    // next
    cur_step++;
    if (cur_step == step_size) {
      cur_step = 0;
      cur_frame++;
    }
  }

 private:
  void computeFrame(int number, bool is_enabled) {
    const int value = is_enabled ? step_size : 0;
    for (int i = 0; i < frame_size; ++i) {
      frames[i][number] = value;
    }
  }

  void computeAppearing(int number) {
    for (int i = 0; i < frame_size - 1; ++i) {
      const int value = (i * step_size) / frame_size;
      frames[i][number] = min(step_size, value);
    }

    frames[frame_size - 1][number] = step_size;
  }

  void computeDisappearing(int number) {
    for (int i = 0; i < frame_size - 1; ++i) {
      const int value = step_size - ((i * step_size) / frame_size);
      frames[i][number] = max(0, value);
    }

    frames[frame_size - 1][number] = 0;
  }
};

#endif //POC_MV57708_SRC_FRAMINGCONTROL_H_
