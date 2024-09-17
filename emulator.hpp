#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include "machine.hpp"

class Emulator {
public:
    Emulator();
    ~Emulator();
    /* Reads the game from disk */
    void readGame(const char *fileName);
private:
    Machine machine;
};

#endif /* EMULATOR_HPP */
