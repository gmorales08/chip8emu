#ifndef INPUT_HPP
#define INPUT_HPP

#include <SDL2/SDL.h>
#include <array>
#include "types.hpp"

class Input {
public:
    static constexpr auto keyMapSize = 16;
    static const std::array<uint8_t, keyMapSize> keyMap;

    Input();
    void handleInput(std::array<byte_t, keyMapSize>& keys);
    bool shouldQuit() const;
private:
    bool quit;
    };

#endif /* INPUT_HPP */
