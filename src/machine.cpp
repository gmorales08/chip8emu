#include <iostream>
#include "machine.hpp"

const std::array<byte_t, Machine::fontsetMemSize> Machine::fontset {
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


Machine::Machine()
    : opcode{}, stack{}, memory{}, v{} ,pc{memPrgStartPos}, sp{}, I{},
      drawFlag{false}, display{}, keys{}, soundFlag{false}, delayTimer{},
      soundTimer{},
      rndGen(std::random_device{}()),
      rndDistrib(rndMinNum, rndMaxNum),
      opcodeTable{&Machine::OPCODE_NULL},
      opcodeTable0{&Machine::OPCODE_NULL},
      opcodeTable8{&Machine::OPCODE_NULL},
      opcodeTableE{&Machine::OPCODE_NULL},
      opcodeTableF{&Machine::OPCODE_NULL} {
    /* Move the fontset to memory */
    for (auto i = 0; i < fontsetMemSize; i++) {
        memory.at(fontsetAddress + i) = fontset.at(i);
    }
    /* Initialize the opcode tables */
    fillOpcodeTables();
}

bool Machine::getDrawFlag() const {
    return drawFlag;
}

void Machine::resetDrawFlag() {
    drawFlag = false;
}

bool Machine::getSoundFlag() const {
    return soundFlag;
}

void Machine::resetSoundFlag() {
    soundFlag = false;
}

void Machine::init() {
    opcode = 0x0000;
    stack.fill(0x0000);
    memory.fill(0x00);
    v.fill(0x00);
    pc = memPrgStartPos;
    sp = 0x0;
    I = 0x0000;
    display.fill(0x00);
    drawFlag = false;
    keys.fill(0x00);
    soundFlag = false;
    delayTimer = 0x00;
    soundTimer = 0x00;
    /* Load the fontset in memory address 0x50 */
    for (auto i = 0; i < fontsetMemSize; i++) {
        memory.at(fontsetAddress + i) = fontset.at(i);
    }
}

void Machine::cycle() {
    /* Fetch */
    opcode = static_cast<word_t>(memory.at(pc) << byteShift) |
             static_cast<word_t>(memory.at(pc + 1));
    std::cout << "PC: " << std::hex << pc << " OPCODE: " << std::hex <<
                 opcode << "\n";

    /* Decode and Execute*/
    ((*this).*(opcodeTable.at(getHighByteHighNibble(opcode))))();

    /* When pc is more than 4095 begins to 0 again */
    pc = pc & (memSize - 1U);

    /* Update timers */
    if (delayTimer > 0) {
        delayTimer--;
    }
    if (soundTimer > 0) {
        soundFlag = true;
        soundTimer--;
    } else {
        soundFlag = false;
    }
    /*std::cout << "Delay Timer: " << delayTimer << "\n";*/
    /*std::cout << "Sound Timer: " << soundTimer << "\n";*/
}

void Machine::loadGame(std::ifstream& ifs) {
    ifs.seekg(0, std::ios::end);
    std::streamsize fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    /* Assert game size is valid */
    if (fileSize > (memSize - memPrgStartPos)) {
        std::cerr << "Memory overflow while loading the ROM" << std::endl;
        exit(1);
    }
    /* Buffer for read file chars and later convert to memory unsigned chars */
    std::vector<char> auxBuf(fileSize);
    /* Load all the file into memory */
    ifs.read(auxBuf.data(), fileSize);
    if (!ifs) {
       std::cerr << "Error reading ROM into buffer" << std::endl;
    }

    /* Move the buffer to memory */
    std::move(auxBuf.begin(), auxBuf.end(), memory.data() + memPrgStartPos);
}

std::array<byte_t, Machine::displaySize>& Machine::getDisplay() {
    return display;
}

std::array<byte_t, Machine::numOfKeys>& Machine::getKeys() {
    return keys;
}

byte_t Machine::rnd() {
    return static_cast<byte_t>(rndDistrib(rndGen));
}

/* Opcode table functions */
void Machine::handleOpcodeTable0() {
    ((*this).*(opcodeTable0.at(getLowByteLowNibble(opcode))))();
}

void Machine::handleOpcodeTable8() {
    ((*this).*(opcodeTable8.at(getLowByteLowNibble(opcode))))();
}

void Machine::handleOpcodeTableE() {
    ((*this).*(opcodeTableE.at(getLowByte(opcode))))();
}

void Machine::handleOpcodeTableF() {
    ((*this).*(opcodeTableF.at(getLowByte(opcode))))();
}

void Machine::fillOpcodeTables() {
    opcodeTable.at(opcodeTable0_idx) = &Machine::handleOpcodeTable0;
    opcodeTable.at(JP_addr_idx) = &Machine::JP_addr;
    opcodeTable.at(CALL_idx) = &Machine::CALL;
    opcodeTable.at(SE_Vx_byte_idx) = &Machine::SE_Vx_byte;
    opcodeTable.at(SNE_Vx_byte_idx) = &Machine::SNE_Vx_byte;
    opcodeTable.at(SE_Vx_Vy_idx) = &Machine::SE_Vx_Vy;
    opcodeTable.at(LD_Vx_byte_idx) = &Machine::LD_Vx_byte;
    opcodeTable.at(ADD_Vx_byte_idx) = &Machine::ADD_Vx_byte;
    opcodeTable.at(opcodeTable8_idx) = &Machine::handleOpcodeTable8;
    opcodeTable.at(SNE_Vx_Vy_idx) = &Machine::SNE_Vx_Vy;
    opcodeTable.at(LD_I_addr_idx) = &Machine::LD_I_addr;
    opcodeTable.at(JP_V0_addr_idx) = &Machine::JP_V0_addr;
    opcodeTable.at(RND_idx) = &Machine::RND;
    opcodeTable.at(DRW_idx) = &Machine::DRW;
    opcodeTable.at(opcodeTableE_idx) = &Machine::handleOpcodeTableE;
    opcodeTable.at(opcodeTableF_idx) = &Machine::handleOpcodeTableF;

    opcodeTable0.at(CLS_idx) = &Machine::CLS;
    opcodeTable0.at(RET_idx) = &Machine::RET;

    opcodeTable8.at(LD_Vx_Vy_idx) = &Machine::LD_Vx_Vy;
    opcodeTable8.at(OR_idx) = &Machine::OR;
    opcodeTable8.at(AND_idx) = &Machine::AND;
    opcodeTable8.at(XOR_idx) = &Machine::XOR;
    opcodeTable8.at(ADD_Vx_Vy_idx) = &Machine::ADD_Vx_Vy;
    opcodeTable8.at(SUB_idx) = &Machine::SUB;
    opcodeTable8.at(SHR_idx) = &Machine::SHR;
    opcodeTable8.at(SUBN_idx) = &Machine::SUBN;
    opcodeTable8.at(SHL_idx) = &Machine::SHL;

    opcodeTableE.at(SKP_idx) = &Machine::SKP;
    opcodeTableE.at(SKNP_idx) = &Machine::SKNP;

    opcodeTableF.at(LD_Vx_DT_idx) = &Machine::LD_Vx_DT;
    opcodeTableF.at(LD_Vx_K_idx) = &Machine::LD_Vx_K;
    opcodeTableF.at(LD_DT_Vx_idx) = &Machine::LD_DT_Vx;
    opcodeTableF.at(LD_ST_Vx_idx) = &Machine::LD_ST_Vx;
    opcodeTableF.at(ADD_I_Vx_idx) = &Machine::ADD_I_Vx;
    opcodeTableF.at(LD_F_Vx_idx) = &Machine::LD_F_Vx;
    opcodeTableF.at(LD_B_Vx_idx) = &Machine::LD_B_Vx;
    opcodeTableF.at(LD_I_Vx_idx) = &Machine::LD_I_Vx;
    opcodeTableF.at(LD_Vx_I_idx) = &Machine::LD_Vx_I;
}

/* Opcode byte operation functions */
constexpr byte_t Machine::getLowByte(word_t opcode) {
    return opcode & lowByteMask;
}

constexpr byte_t Machine::getHighByteHighNibble(word_t opcode) {
     return static_cast<word_t>(opcode & highByteHighNibbleMask) >>
         byteAndNibbleShift;
}

constexpr byte_t Machine::getHighByteLowNibble(word_t opcode) {
    return static_cast<word_t>(opcode & highByteLowNibbleMask) >> byteShift;
}

constexpr byte_t Machine::getLowByteHighNibble(word_t opcode) {
    return opcode & lowByteHighNibbleMask;
}

constexpr byte_t Machine::getLowByteLowNibble(word_t opcode) {
    return opcode & lowByteLowNibbleMask;
}

constexpr word_t Machine::get_nnn(word_t opcode) {
    return opcode & nnnMask;
}

constexpr byte_t Machine::get_n(word_t opcode) {
    return opcode & nMask;
}

constexpr byte_t Machine::get_x(word_t opcode) {
    return static_cast<word_t>(opcode & xMask) >> byteShift;
}

constexpr byte_t Machine::get_y(word_t opcode) {
    return static_cast<word_t>(opcode & yMask) >> nibbleShift;
}

constexpr byte_t Machine::get_kk(word_t opcode) {
    return opcode & kkMask;
}

/* Instruction functions */

void Machine::CLS() {
    display.fill(0x0);
    pc = pc + 2;
}

void Machine::RET() {
    pc = stack.at(sp);
    sp--;
    pc = pc + 2;
}

/*void Machine::SYS() {*/
    /*[> SYS addr <]*/
    /*[> Not used by this interpreter <]*/
    /*pc = pc + 2;*/
/*}*/

void Machine::JP_addr() {
    /* JP addr */
    pc = get_nnn(opcode);
}

void Machine::CALL() {
    /* CALL addr */
    sp++;
    stack.at(sp) = pc;
    pc = get_nnn(opcode);
}

void Machine::SE_Vx_byte() {
    /* SE Vx, byte */
    if (v.at(get_x(opcode)) == get_kk(opcode)) {
        pc = pc + 4;
    } else {
        pc = pc + 2;
    }
}

void Machine::SNE_Vx_byte() {
    /* SNE Vx, byte */
    if (v.at(get_x(opcode)) != get_kk(opcode)) {
        pc = pc + 4;
    } else {
        pc = pc + 2;
    }
}

void Machine::SE_Vx_Vy() {
    /* SE Vx, Vy */
    if (v.at(get_x(opcode)) == v.at(get_y(opcode))) {
        pc = pc + 4;
    } else {
        pc = pc + 2;
    }
}

void Machine::LD_Vx_byte() {
    /* LD Vx, byte */
    v.at(get_x(opcode)) = get_kk(opcode);
    pc = pc + 2;
}

void Machine::ADD_Vx_byte() {
    /* ADD Vx, byte */
    v.at(get_x(opcode)) = v.at(get_x(opcode)) + get_kk(opcode);
    pc = pc + 2;
}

void Machine::LD_Vx_Vy() {
    /* LD Vx, Vy */
    v.at(get_x(opcode)) = v.at(get_y(opcode));
    pc = pc + 2;
}

void Machine::OR() {
    /* OR Vx, Vy */
    v.at(get_x(opcode)) = v.at(get_x(opcode)) | v.at(get_y(opcode));
    pc = pc + 2;
}

void Machine::AND() {
    /* AND Vx, Vy */
    v.at(get_x(opcode)) = v.at(get_x(opcode)) & v.at(get_y(opcode));
    pc = pc + 2;
}

void Machine::XOR() {
    /* XOR Vx, Vy */
    v.at(get_x(opcode)) = v.at(get_x(opcode)) ^ v.at(get_y(opcode));
    pc = pc + 2;
}

void Machine::ADD_Vx_Vy() {
    /* ADD Vx, Vy */
    static constexpr auto maxByteValue = 255;
    word_t sum = v.at(get_x(opcode)) + v.at(get_y(opcode));
    v.at(get_x(opcode)) = getLowByte(sum);
    v.at(VF_idx) = sum > maxByteValue ? 1 : 0;
    pc = pc + 2;
}

void Machine::SUB() {
    /* SUB Vx, Vy */
    bool borrow = v.at(get_x(opcode)) < v.at(get_y(opcode));
    v.at(get_x(opcode)) = v.at(get_x(opcode)) - v.at(get_y(opcode));
    if (borrow) {
        v.at(VF_idx) = 0;
    } else {
        v.at(VF_idx) = 1;
    }
    pc = pc + 2;
}

void Machine::SHR() {
    /* SHR Vx {, Vy} */
    static constexpr auto lowBitMask = 0x01U;
    auto lowBitOfVx = v.at(get_x(opcode)) & lowBitMask;
    v.at(get_x(opcode)) = v.at(get_x(opcode)) >>= 1U;
    v.at(VF_idx) = lowBitOfVx;
    pc = pc + 2;
}

void Machine::SUBN() {
    /* SUBN Vx, Vy */
    bool borrow = v.at(get_y(opcode)) < v.at(get_x(opcode));
    v.at(get_x(opcode)) = v.at(get_y(opcode)) - v.at(get_x(opcode));
    if (borrow) {
        v.at(VF_idx) = 0;
    } else {
        v.at(VF_idx) = 1;
    }
    pc = pc + 2;
}

void Machine::SHL() {
    /* SHL Vx {, Vy} */
    static constexpr auto highBitMask = 0x80U;
    static constexpr auto highBitShift = 7U;
    auto highBitOfVx = (v.at(get_x(opcode)) & highBitMask) >> highBitShift;
    v.at(get_x(opcode)) = v.at(get_x(opcode)) <<= 1U;
    v.at(VF_idx) = highBitOfVx;
    pc = pc + 2;
}

void Machine::SNE_Vx_Vy() {
    /* SNE Vx, Vy */
    if (v.at(get_x(opcode)) != v.at(get_y(opcode))) {
        pc = pc + 4;
    } else {
        pc = pc + 2;
    }
}

void Machine::LD_I_addr() {
    /* LD I, addr */
    I = get_nnn(opcode);
    pc = pc + 2;
}

void Machine::JP_V0_addr() {
    /* JP V0, addr */
    pc = get_nnn(opcode) + v.at(V0_idx);
}

void Machine::RND() {
    /* RND Vx, byte */
    v.at(get_x(opcode)) = rnd() & get_kk(opcode);
    pc = pc + 2;
}

void Machine::DRW() {
    /* DRW Vx, Vy, nibble */
    static constexpr auto numberOfLines = 8;
    static constexpr byte_t pixelMask = 0x80;

    bool pixel = false;
    /* VF is set when a collision occurs */
    v.at(VF_idx) = 0x0;
    for (auto yline = 0U; yline < get_n(opcode); yline++) {
        for (auto xline = 0U; xline < numberOfLines; xline++) {
            pixel = (memory.at(I + yline) &
                    static_cast<byte_t>(pixelMask >> xline)) != 0;
            if (pixel) {
                auto idx =
                    (v.at(get_x(opcode)) + xline) % displayWidth +
                    ((v.at(get_y(opcode)) + yline) % displayHeight) *
                    displayWidth;
                if (display.at(idx) != 0) {
                    v.at(VF_idx) = 0x1;
                    display.at(idx) = 0;
                } else {
                    display.at(idx) = 1;
                }
            }
        }
    }
    drawFlag = true;
    pc = pc + 2;
}

void Machine::SKP() {
    /* SKP Vx */
    if (keys.at(v.at(get_x(opcode))) != 0) {
        pc = pc + 4;
    } else {
        pc = pc + 2;
    }
}

void Machine::SKNP() {
    /* SKNP Vx */
    if (keys.at(v.at(get_x(opcode))) == 0) {
        pc = pc + 4;
    } else {
        pc = pc + 2;
    }
}

void Machine::LD_Vx_DT() {
    /* LD Vx, DT */
    v.at(get_x(opcode)) = delayTimer;
    pc = pc + 2;
}

void Machine::LD_Vx_K() {
    /* LD Vx, K */
    bool keyPressed = false;
    for (byte_t key : keys) {
        if (key != 0) {
            keyPressed = true;
            v.at(get_x(opcode)) = key;
        }
    }
    if (keyPressed) {
        pc = pc + 2;
    }
}

void Machine::LD_DT_Vx() {
    /* LD DT, Vx */
    delayTimer = v.at(get_x(opcode));
    pc = pc + 2;
}

void Machine::LD_ST_Vx() {
    /* LD ST, Vx */
    soundTimer = v.at(get_x(opcode));
    pc = pc + 2;
}

void Machine::ADD_I_Vx() {
    /* ADD I, Vx */
    I = I + v.at(get_x(opcode));
    pc = pc + 2;
}

void Machine::LD_F_Vx() {
    /* LD F, Vx */
    I = fontsetAddress + v.at(get_x(opcode)) * fontCharHeight;
    pc = pc + 2;
}

void Machine::LD_B_Vx() {
    /* LD B, Vx */
    static constexpr auto hundredsDivisor = 100;
    static constexpr auto tensDivisor = 10;
    static constexpr auto digitMod = 10;

    memory.at(I) = v.at(get_x(opcode)) / hundredsDivisor;
    memory.at(I + 1) = (v.at(get_x(opcode)) / tensDivisor) % digitMod;
    memory.at(I + 2) = v.at(get_x(opcode)) % digitMod;
    pc = pc + 2;
}

void Machine::LD_I_Vx() {
    /* Fx55 - LD [I], Vx */
    for (int idx = 0; idx <= get_x(opcode); idx++) {
        memory.at(I + idx) = v.at(idx);
    }
    /* I is incremented in the specification */
    I = I + get_x(opcode) + 1;
    pc = pc + 2;
}

void Machine::LD_Vx_I() {
    /* LD Vx, [I] */
    for (int idx = 0; idx <= get_x(opcode); idx++) {
        v.at(idx) = memory.at(I + idx);
    }
    /* I is incremented in the specification */
    I = I + get_x(opcode) + 1;
    pc = pc + 2;
}

void Machine::OPCODE_NULL() {
    /* OPCODE not recognised */
}
