#ifndef MACHINE_HPP
#define MACHINE_HPP

#include <array>
#include <fstream>
#include <random>
#include "types.hpp"


/* Size of the machine memory */
#define MEM_SIZE 4096
/* Memory start address of the programs */
#define MEM_PRG_START_POS 0x200
/* Size of the stack (in words) */
#define STACK_SIZE 16


const byte_t fontset[80] {
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


class Machine {
public:
    Machine();
    ~Machine();
    void init();
    void cycle();
    /* Loads the game to memory. Needs the pointer to the game file
     * on disk */
    void loadGame(std::ifstream& ifs);
    std::array<byte_t, 64 * 32>& getDisplay();
    /* Set when the display need to be updated */
    int drawFlag;
    std::array<byte_t, 16>& getKeys();
private:
    word_t opcode;
    std::array<word_t, STACK_SIZE> stack;
    std::array<byte_t, MEM_SIZE> memory;

    std::array<byte_t, 16> v; /* Registers v0-vF */
    word_t pc;
    word_t sp;
    word_t I;

    std::array<byte_t, 64 * 32> display; /* Graphic memory buffer */
    std::array<byte_t, 16> keys; /* Key status */

    byte_t delayTimer;
    byte_t soundTimer;

    std::mt19937 rndGen;
    std::uniform_int_distribution<> rndDistrib;
    /* Generates a random number in range 0-255 */
    byte_t rnd();
};

#endif
