#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <SDL2/SDL.h>
#include <array>
#include "types.hpp"

class Graphics {
public:
    Graphics();
    ~Graphics();
    void init();
    void render(std::array<byte_t, 64 * 32>& displayMem);
    void clear();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};

#endif /* GRAPHICS_HPP */
