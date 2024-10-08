#include <iostream>
#include <fstream>
#include "emulator.hpp"


Emulator::Emulator() {
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

Graphics& Emulator::getGraphics() {
    return graphics;
}

Machine& Emulator::getMachine() {
    return machine;
}
