#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <SDL2/SDL.h>
#include <array>
#include "types.hpp"
#include "machine.hpp"

class Graphics {
public:
    Graphics();
    ~Graphics();

    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;
    Graphics(Graphics&&) = delete;
    Graphics& operator=(Graphics&&) = delete;

    void init();
    void render(std::array<byte_t, Machine::displaySize>& displayMem);
    void clear();
private:
    static constexpr auto windowWidth = Machine::displayWidth * 10;
    static constexpr auto windowHeight = Machine::displayHeight * 10;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};

#endif /* GRAPHICS_HPP */
