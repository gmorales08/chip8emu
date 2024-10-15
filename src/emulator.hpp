#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include "graphics.hpp"
#include "input.hpp"
#include "sound.hpp"

class Emulator {
public:
    explicit Emulator(uint16_t speed);
    /* Reads the game from disk */
    void readGame(const char *fileName);
    void run();
private:
    uint16_t speed;
    Graphics graphics;
    Input input;
    Sound sound;
    Machine machine;
};

#endif /* EMULATOR_HPP */
