#ifndef POC_SERIALCONTROL_SRC_SERIALCONTROL_H_
#define POC_SERIALCONTROL_SRC_SERIALCONTROL_H_

#include <Arduino.h>
#include <unordered_map>
#include <iostream>
#include <functional>
#include <utility>

typedef std::function<void(std::string &output, std::string &args)> CommandCallback;

class SerialControl {
 private:
  std::string buffer;
  std::unordered_map<std::string, CommandCallback> commandToCallback;

 public:
  void add(const char *s, CommandCallback callback) {
    auto command = std::string(s);
    add(command, std::move(callback));
  }

  void add(std::string &command, CommandCallback callback) {
    commandToCallback[command] = std::move(callback);
  }

  std::string push(String &s) {
    std::string output;

    for (char c : s) {
      if (c == '\n') {
        // trigger command
        output += '\n';
        output += "> ";

        trigger(output, buffer);
        output += '\n';

        buffer.clear();

      } else if (c == '\b') {
        // backspace (only effective if buffer is not empty)
        if (!buffer.empty()) {
          buffer.pop_back();
          output += '\b';
        }

      } else if (c >= ' ') {
        // non-special char
        buffer += c;
        output += c;
      }
    }

    return output;
  }

 private:
  void trigger(std::string &output, std::string &input) {
    // <command> <args>
    std::string command;
    std::string args;

    extract(command, args, input);

    // find the command
    if (!commandToCallback.count(command)) {
      output += "command not found\n";
      return;
    }

    auto &callback = commandToCallback[command];
    callback(output, args);
  }

  static void extract(std::string &command, std::string &args, std::string &input) {
    const unsigned int n = input.size();

    int i = 0;
    while (i < n && input[i] != ' ') {
      command += input[i];
      i++;
    }

    while (i < n && input[i] == ' ') {
      i++;
    }

    while (i < n) {
      args += input[i];
      i++;
    }
  }
};

#endif //POC_SERIALCONTROL_SRC_SERIALCONTROL_H_
