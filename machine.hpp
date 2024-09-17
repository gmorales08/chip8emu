#ifndef MACHINE_HPP
#define MACHINE_HPP

#include <cstdint>
#include <array>
#include <fstream>
#include <random>


/* Size of the machine memory */
#define MEM_SIZE 4096
/* Memory start address of the programs */
#define MEM_PRG_START_POS 0x200
/* Size of the stack (in words) */
#define STACK_SIZE 16


typedef uint8_t byte_t;
typedef uint16_t word_t;

class Machine {
public:
    Machine();
    ~Machine();
    void init();
    void cycle();
    /* Loads the game to memory. Needs the pointer to the game file
     * on disk */
    void loadGame(std::ifstream& ifs);
private:
    word_t opcode;
    std::array<word_t, STACK_SIZE> stack;
    std::array<byte_t, MEM_SIZE> memory;

    std::array<byte_t, 16> v; /* Registers v0-vF */
    word_t pc;
    word_t sp;
    word_t i;

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
