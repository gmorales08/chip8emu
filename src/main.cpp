#include <SDL2/SDL.h>
#include <iostream>
#include "emulator.hpp"

int main(int argc, char** argv) {
    Emulator emulator;
    emulator.readGame(argv[1]);

    /*for (int i = 0; i < emulator.getMachine().getDisplay().size(); i++) {*/
        /*printf("%i ", emulator.getMachine().getDisplay().at(i));*/
        /*if (i % 64 == 0) std::cout << "\n";*/
    /*}*/

    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;
        }
        emulator.getMachine().cycle();
        if (emulator.getMachine().drawFlag) {
            emulator.getGraphics().render(emulator.getMachine().getDisplay());
            emulator.getMachine().drawFlag = 0;
            /*std::cout << "\n===\n";*/
            /*for (int i = 0; i < emulator.getMachine().getDisplay().size(); i++) {*/
                /*printf("%i ", emulator.getMachine().getDisplay().at(i));*/
                /*if (i % 64 == 0) std::cout << "\n";*/
            /*}*/
        }
        SDL_Delay(16);
    }

    return 0;
}
