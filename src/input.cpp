#include "input.hpp"

const std::array<uint8_t, Input::keyMapSize> Input::keyMap = {
        SDLK_x, SDLK_1, SDLK_2, SDLK_3,
        SDLK_q, SDLK_w, SDLK_e, SDLK_a,
        SDLK_s, SDLK_d, SDLK_z, SDLK_c,
        SDLK_4, SDLK_r, SDLK_f, SDLK_v,
};

Input::Input() : quit{false} {}

void Input::handleInput(std::array<byte_t, keyMapSize>& keys) {
    SDL_Event event {};
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            quit = true;
        }
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            for (auto i = 0; i < keyMapSize; i++) {
                if (event.key.keysym.sym == keyMap.at(i)) {
                    keys.at(i) = (event.type == SDL_KEYDOWN) ? 1 : 0;
                }
            }
        }
    }
}

bool Input::shouldQuit() const {
    return quit;
}
