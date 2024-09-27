#include <SDL2/SDL.h>
#include <iostream>
#include "emulator.hpp"

void testKeyboard(std::array<byte_t, 16>& keys) {

}

int main(int argc, char** argv) {
    Emulator emulator;
    emulator.readGame(argv[1]);

    /*for (int i = 0; i < emulator.getMachine().getDisplay().size(); i++) {*/
        /*printf("%i ", emulator.getMachine().getDisplay().at(i));*/
        /*if (i % 64 == 0) std::cout << "\n";*/
    /*}*/
uint8_t KEYMAP[16] = {
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
    Uint32 frameStart, frameTime;
    while (!quit) {
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                        quit = true;
                }
                for (int i = 0; i < emulator.getMachine().getKeys().size();
                        i++) {
                    if (event.key.keysym.sym == KEYMAP[i])
                        emulator.getMachine().getKeys().at(i) = 1;
                }
            } else if (event.type == SDL_KEYUP) {
                for (int i = 0; i < emulator.getMachine().getKeys().size();
                        i++) {
                    if (event.key.keysym.sym == KEYMAP[i])
                        emulator.getMachine().getKeys().at(i) = 0;
                }
            }
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
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }

    return 0;
}
