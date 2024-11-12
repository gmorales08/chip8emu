#ifndef KEYBOARD_CONTROLLER_HPP
#define KEYBOARD_CONTROLLER_HPP

#include <SDL2/SDL.h>

#include <array>

#include "../bus.hpp"
#include "../types.hpp"

class KeyboardController {
 public:
  static constexpr auto keyMapSize = 16;
  static const std::array<uint8_t, keyMapSize> keyMap;

  KeyboardController(Bus& bus);
  void scanKeyboard();
  static bool shouldQuit();

 private:
  Bus& bus;
  bool quit;
};

#endif /* KEYBOARD_CONTROLLER_HPP */
