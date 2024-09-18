#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include "machine.hpp"
#include "graphics.hpp"

class Emulator {
public:
    Emulator();
    ~Emulator();
    /* Reads the game from disk */
    void readGame(const char *fileName);
    Graphics& getGraphics();
    Machine& getMachine();
private:
    Graphics graphics;
    Machine machine;
};

#endif /* EMULATOR_HPP */
