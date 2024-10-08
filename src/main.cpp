#include <SDL2/SDL.h>
#include <iostream>
#include "emulator.hpp"

int main(int argc, char** argv) {
    Emulator emulator;
    if (argc == 2) {
        emulator.readGame(argv[1]);
    } else {
        exit(0);
    }

    static constexpr auto keyMapSize = 16;
    static constexpr std::array<uint8_t, keyMapSize> keyMap = {
        SDLK_x,
        SDLK_1,
        SDLK_2,
        SDLK_3,
        SDLK_q,
        SDLK_w,
        SDLK_e,
        SDLK_a,
        SDLK_s,
        SDLK_d,
        SDLK_z,
        SDLK_c,
        SDLK_4,
        SDLK_r,
        SDLK_f,
        SDLK_v,
    };
    bool quit = false;
    SDL_Event event;
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart {};
    Uint32 frameTime {};

    int cycles = 0;

    while (!quit) {
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                        quit = true;
                }
                for (auto i = 0; i < emulator.getMachine().getKeys().size();
                        i++) {
                    if (event.key.keysym.sym == keyMap.at(i)) {
                        emulator.getMachine().getKeys().at(i) = 1;
                    }
                }
            } else if (event.type == SDL_KEYUP) {
                for (auto i = 0U; i < emulator.getMachine().getKeys().size();
                        i++) {
                    if (event.key.keysym.sym == keyMap.at(i)) {
                        emulator.getMachine().getKeys().at(i) = 0;
                    }
                }
            }
        }
        emulator.getMachine().cycle();
        cycles++;
        std::cout << "Cycles: " << std::dec <<  cycles << std::endl;
        if (emulator.getMachine().getDrawFlag()) {
            emulator.getGraphics().render(emulator.getMachine().getDisplay());
            emulator.getMachine().resetDrawFlag();
        }
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    return 0;
}
