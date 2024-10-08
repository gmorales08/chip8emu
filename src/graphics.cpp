#include <stdexcept>
#include "graphics.hpp"


Graphics::Graphics()
    : window{nullptr}, renderer{nullptr}, texture{nullptr} {
}

Graphics::~Graphics() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Graphics::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error("Failed to initialize SDL2");
    }

    window = SDL_CreateWindow("chip8emu",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              windowWidth,
                              windowHeight,
                              SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        throw std::runtime_error("Failed to create window");
    }

    renderer = SDL_CreateRenderer(window,
                                  -1,
                                  SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        throw std::runtime_error("Failed to create renderer");
    }

    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_RGB888,
                                SDL_TEXTUREACCESS_STREAMING,
                                Machine::displayWidth,
                                Machine::displayHeight);
    if (texture == nullptr) {
        throw std::runtime_error("Failed to create texture");
    }
}

void Graphics::render(std::array<byte_t, Machine::displaySize>& displayMem) {
    static constexpr auto pixelOffValue = 0x000000;
    static constexpr auto pixelOnValue = 0xFFFFFF;
    std::array<uint32_t, Machine::displaySize> pixels {};

    for (auto i = 0U; i < displayMem.size(); i++) {
        pixels.at(i) = (displayMem.at(i) == 0) ? pixelOffValue : pixelOnValue;
    }

    const void* displayData = static_cast<const void*>(pixels.data());
    SDL_UpdateTexture(texture, nullptr, displayData, Machine::displayWidth *
            sizeof(uint32_t));

    SDL_RenderClear(renderer);

    /* Scale the resolution */
    SDL_Rect dstRect = {0, 0, windowWidth, windowHeight};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

    SDL_RenderPresent(renderer);
}

void Graphics::clear() {
    SDL_RenderClear(renderer);
}


