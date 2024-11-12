#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include "machine.hpp"
#include "sdl/graphics.hpp"
#include "sdl/keyboardController.hpp"
#include "sdl/sound.hpp"

class Emulator {
 public:
  explicit Emulator(uint32_t speed);
  /* Reads the game from disk */
  void readGame(const char *fileName);
  void run();

 private:
  double speed;
  Machine machine;
  Graphics graphics;
  Sound sound;
};

#endif /* EMULATOR_HPP */
