#include <iostream>
#include <fstream>
#include "emulator.hpp"


Emulator::Emulator(uint16_t _speed) : speed{_speed} {
    graphics.init();
}

void Emulator::readGame(const char* fileName) {
    std::ifstream ifs {fileName, std::ios_base::binary};
    if (!ifs.is_open()) {
        std::cerr << "Couldn't open file for reading" << std::endl;
        exit(1);
    }
    std::cout << "Game file readed" << std::endl;
    machine.loadGame(ifs);
}

void Emulator::run() {
    const Uint32 frameDelay = 1000 / speed;
    Uint32 frameStart {};
    Uint32 frameTime {};

    int cycles = 0;
    while (!input.shouldQuit()) {
        frameStart = SDL_GetTicks();
        input.handleInput(machine.getKeys());
        machine.cycle();

        if (machine.getDrawFlag()) {
            graphics.render(machine.getDisplay());
            machine.resetDrawFlag();
        }

        if (machine.getSoundFlag()) {
            sound.playBeep();
        } else {
            sound.stopBeep();
        }

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }

        cycles++;
    }
}
