#include "emulator.hpp"

int main(int argc, char** argv) {
    constexpr uint16_t emulatorSpeed = 1000;
    Emulator emulator(emulatorSpeed);
    if (argc == 2) {
        emulator.readGame(argv[1]);
    } else {
        exit(0);
    }
    emulator.run();

    return 0;
}
