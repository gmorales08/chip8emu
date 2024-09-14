#ifndef MACHINE_HPP
#define MACHINE_HPP

#include <cstdint>
#include <array>

typedef uint8_t byte_t;
typedef uint16_t word_t;

class Machine {
public:
    Machine();
    ~Machine();
    void cycle();
private:
    word_t opcode;
    std::array<word_t, 16> stack;
    std::array<byte_t, 4096> memory;

    std::array<byte_t, 16> v; /* Registers v0-vF */
    byte_t pc;
    word_t i;

    std::array<byte_t, 64 * 32> display;
    std::array<byte_t, 16> keys;

    byte_t delayTimer;
    byte_t soundTimer;
};

#endif
