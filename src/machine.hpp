#ifndef MACHINE_HPP
#define MACHINE_HPP

#include <array>
#include <fstream>
#include <random>
#include "types.hpp"


class Machine {
public:
    static constexpr auto displayWidth = 64;
    static constexpr auto displayHeight = 32;
    static constexpr auto displaySize = displayWidth * displayHeight;
    static constexpr auto numOfKeys = 16;

    Machine();
    /* Start the machine manually */
    void init();
    void cycle();
    /* Loads the game to memory. Needs the pointer to the game file
     * on disk */
    void loadGame(std::ifstream& ifs);
    std::array<byte_t, displaySize>& getDisplay();
    /* Set when the display need to be updated */
    bool getDrawFlag() const;
    void resetDrawFlag();
    std::array<byte_t, Machine::numOfKeys>& getKeys();
    bool getSoundFlag() const;
    void resetSoundFlag();
private:
    /* Constants */
    /* Size of the machine memory (in bytes) */
    static constexpr auto memSize = 4096;
    /* Memory start address of the programs */
    static constexpr auto memPrgStartPos = 0x200;
    /* Size of the stack (in words) */
    static constexpr auto stackSize = 16;

    static constexpr auto fontsetMemSize = 80;
    static const std::array<byte_t, fontsetMemSize> fontset;
    /* Number of rows of each font character */
    static constexpr auto fontCharHeight = 5;

   /* Memory position of the first font character */
    static constexpr auto fontsetAddress = 0x50;

    static constexpr auto numOfVRegs = 16;
    static constexpr auto V0_idx = 0x0;
    static constexpr auto VF_idx = 0xF;

    /* Range for the RND instruction */
    static constexpr auto rndMinNum = 0;
    static constexpr auto rndMaxNum = 255;

    /* Size of the opcode tables */
    static constexpr auto nibbleTableSize = 0xF + 1;
    static constexpr auto byteTableSize = 0xFF + 1;

    /* Masks for operate with the opcodes */
    static constexpr word_t lowByteMask = 0x00FF;
    static constexpr word_t highByteHighNibbleMask = 0xF000;
    static constexpr word_t highByteLowNibbleMask = 0x0F00;
    static constexpr word_t lowByteHighNibbleMask = 0x00F0;
    static constexpr word_t lowByteLowNibbleMask = 0x000F;
    static constexpr word_t nnnMask = 0x0FFF;
    static constexpr word_t nMask = 0x000F;
    static constexpr word_t xMask = 0x0F00;
    static constexpr word_t yMask = 0x00F0;
    static constexpr word_t kkMask = 0x00FF;
    /* Constants for the shift operations */
    static constexpr uint8_t nibbleShift = 4;
    static constexpr uint8_t byteShift = 8;
    static constexpr uint8_t byteAndNibbleShift = 12;
    /* Constants for opcode table index */
    static constexpr auto opcodeTable0_idx = 0x0;
    static constexpr auto JP_addr_idx = 0x1;
    static constexpr auto CALL_idx = 0x2;
    static constexpr auto SE_Vx_byte_idx = 0x3;
    static constexpr auto SNE_Vx_byte_idx = 0x4;
    static constexpr auto SE_Vx_Vy_idx = 0x5;
    static constexpr auto LD_Vx_byte_idx = 0x6;
    static constexpr auto ADD_Vx_byte_idx = 0x7;
    static constexpr auto opcodeTable8_idx = 0x8;
    static constexpr auto SNE_Vx_Vy_idx = 0x9;
    static constexpr auto LD_I_addr_idx = 0xA;
    static constexpr auto JP_V0_addr_idx = 0xB;
    static constexpr auto RND_idx = 0xC;
    static constexpr auto DRW_idx = 0xD;
    static constexpr auto opcodeTableE_idx = 0xE;
    static constexpr auto opcodeTableF_idx = 0xF;
    /* Constants for opcode table 0 index */
    static constexpr auto CLS_idx = 0x0;
    static constexpr auto RET_idx = 0xE;
    /* Constants for opcode table 8 index */
    static constexpr auto LD_Vx_Vy_idx = 0x0;
    static constexpr auto OR_idx = 0x1;
    static constexpr auto AND_idx = 0x2;
    static constexpr auto XOR_idx = 0x3;
    static constexpr auto ADD_Vx_Vy_idx = 0x4;
    static constexpr auto SUB_idx = 0x5;
    static constexpr auto SHR_idx = 0x6;
    static constexpr auto SUBN_idx = 0x7;
    static constexpr auto SHL_idx = 0xE;
    /* Constants for opcode table E index */
    static constexpr auto SKP_idx = 0x9E;
    static constexpr auto SKNP_idx = 0xA1;
    /* Constants for opcode table F index */
    static constexpr auto LD_Vx_DT_idx = 0x07;
    static constexpr auto LD_Vx_K_idx = 0x0A;
    static constexpr auto LD_DT_Vx_idx = 0x15;
    static constexpr auto LD_ST_Vx_idx = 0x18;
    static constexpr auto ADD_I_Vx_idx = 0x1E;
    static constexpr auto LD_F_Vx_idx = 0x29;
    static constexpr auto LD_B_Vx_idx = 0x33;
    static constexpr auto LD_I_Vx_idx = 0x55;
    static constexpr auto LD_Vx_I_idx = 0x65;


    word_t opcode;
    std::array<word_t, stackSize> stack;
    std::array<byte_t, memSize> memory;

    std::array<byte_t, numOfVRegs> v; /* Registers v0-vF */
    word_t pc;
    word_t sp;
    word_t I;

    bool drawFlag;
    std::array<byte_t, displaySize> display; /* Graphic memory buffer */
    std::array<byte_t, numOfKeys> keys; /* Key status */

    bool soundFlag;

    byte_t delayTimer;
    byte_t soundTimer;

    std::mt19937 rndGen;
    std::uniform_int_distribution<> rndDistrib;

    /* Alias for function pointers */
    typedef void (Machine::*Instruction)();

    /* Opcode tables */
    std::array<Instruction, nibbleTableSize> opcodeTable;
    std::array<Instruction, nibbleTableSize> opcodeTable0;
    std::array<Instruction, nibbleTableSize> opcodeTable8;
    std::array<Instruction, byteTableSize> opcodeTableE;
    std::array<Instruction, byteTableSize> opcodeTableF;
    void handleOpcodeTable0();
    void handleOpcodeTable8();
    void handleOpcodeTableE();
    void handleOpcodeTableF();
    /* Fills the opcode tables with the opcode function pointers */
    void fillOpcodeTables();

    /* Generates a random number in range 0-255 */
    byte_t rnd();

    /* Functions for opcode byte operations */
    /* Opcode: 0000111122223333
     * 00001111: high byte
     * 22223333: low byte
     * 0000: high byte high nibble
     * 1111: high byte low nibble
     * 2222: low byte high nibble
     * 3333: low byte low nibble */
    static constexpr byte_t getLowByte(word_t opcode);
    static constexpr byte_t getHighByteHighNibble(word_t opcode);
    static constexpr byte_t getHighByteLowNibble(word_t opcode);
    static constexpr byte_t getLowByteHighNibble(word_t opcode);
    static constexpr byte_t getLowByteLowNibble(word_t opcode);

    static constexpr word_t get_nnn(word_t opcode);
    static constexpr byte_t get_n(word_t opcode);
    static constexpr byte_t get_x(word_t opcode);
    static constexpr byte_t get_y(word_t opcode);
    static constexpr byte_t get_kk(word_t opcode);

    /* Functions for intructions */
    void CLS();
    void RET();
    //void SYS(); Not used by this interpreter
    void JP_addr();
    void CALL();
    void SE_Vx_byte();
    void SNE_Vx_byte();
    void SE_Vx_Vy();
    void LD_Vx_byte();
    void ADD_Vx_byte();
    void LD_Vx_Vy();
    void OR();
    void AND();
    void XOR();
    void ADD_Vx_Vy();
    void SUB();
    void SHR();
    void SUBN();
    void SHL();
    void SNE_Vx_Vy();
    void LD_I_addr();
    void JP_V0_addr();
    void RND();
    void DRW();
    void SKP();
    void SKNP();
    void LD_Vx_DT();
    void LD_Vx_K();
    void LD_DT_Vx();
    void LD_ST_Vx();
    void ADD_I_Vx();
    void LD_F_Vx();
    void LD_B_Vx();
    void LD_I_Vx();
    void LD_Vx_I();
    void OPCODE_NULL();
};

#endif
