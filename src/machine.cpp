#include <iostream>
#include "machine.hpp"

Machine::Machine()
    : rndGen(std::random_device{}()),
      rndDistrib(0, 255) {
}

Machine::~Machine() {

}

void Machine::init() {
    opcode =  0x0000;
    stack.fill(0x0000);
    memory.fill(0x00);
    v.fill(0x00);
    pc = MEM_PRG_START_POS;
    sp = 0x0;
    I = 0x0000;
    display.fill(0x00);
    drawFlag = 0;
    keys.fill(0x00);
    delayTimer = 0x00;
    soundTimer = 0x00;
    /* Load the fontset in memory address 0x50 */
    for (int i = 0; i < 80; i++)
        memory.at(0x50 + i) = fontset[i];
}

void Machine::cycle() {
    /* Fetch */
    opcode = memory.at(pc) << 8 | memory.at(pc + 1);
    std::cout << "PC: " << std::hex << pc << " OPCODE: " << std::hex <<
                 opcode << "\n";

    /* Decode and Execute*/
    word_t nnn = opcode & 0x0FFF;
    word_t n   = opcode & 0x000F;
    word_t x   = (opcode & 0x0F00) >> 8;
    word_t y   = (opcode & 0x00F0) >> 4;
    word_t kk  = opcode & 0x00FF;
    switch (opcode >> 12) {
        case 0x0:
            switch (opcode) {
                case 0x00E0:
                    /* CLS */
                    display.fill(0x0);
                    pc = pc + 2;
                    break;
                case 0x00EE:
                    /* RET */
                    pc = stack.at(sp);
                    sp--;
                    pc = pc + 2;
                    break;
                default: /* 0nnn */
                    /* SYS addr */
                    /* Not used by this interpreter */
                    pc = pc + 2;
                    break;
            }
            break;
        case 0x1:
            /* JP addr */
            pc = nnn;
            break;
        case 0x2:
            /* CALL addr */
            sp++;
            stack.at(sp) = pc;
            pc = nnn;
            break;
        case 0x3:
            /* SE Vx, byte */
            if (v.at(x) == kk)
                pc = pc + 4;
            else
                pc = pc + 2;
            break;
        case 0x4:
            /* SNE Vx, byte */
            if (v.at(x) != kk)
                pc = pc + 4;
            else
                pc = pc + 2;
            break;
        case 0x5:
            /* SE Vx, Vy */
            if (v.at(x) == v.at(y))
                pc = pc + 4;
            else
                pc = pc + 2;
            std::cout << "PC: " << pc << "\n";
            break;
        case 0x6:
            /* LD Vx, byte */
            v.at(x) = kk;
            pc = pc + 2;
            break;
        case 0x7:
            /* ADD Vx, byte */
            v.at(x) = v.at(x) + kk;
            pc = pc + 2;
            break;
        case 0x8:
            switch (opcode & 0x000F) {
                case 0x0:
                    /* LD Vx, Vy */
                    v.at(x) = v.at(y);
                    pc = pc + 2;
                    break;
                case 0x1:
                    /* OR Vx, Vy */
                    v.at(x) = v.at(x) | v.at(y);
                    pc = pc + 2;
                    break;
                case 0x2:
                    /* AND Vx, Vy */
                    v.at(x) = v.at(x) & v.at(y);
                    pc = pc + 2;
                    break;
                case 0x3:
                    /* XOR Vx, Vy */
                    v.at(x) = v.at(x) ^ v.at(y);
                    pc = pc + 2;
                    break;
                case 0x4: {
                    /* ADD Vx, Vy */
                    word_t sum = v.at(x) + v.at(y);
                    v.at(x) = sum & 0x00FF;
                    v.at(0xF) = sum > 255 ? 1 : 0;
                    pc = pc + 2;
                    break;
                }
                case 0x5:
                    /* SUB Vx, Vy */
                    v.at(0xF) = v.at(x) > v.at(y) ? 1 : 0;
                    v.at(x) = v.at(x) - v.at(y);
                    pc = pc + 2;
                    break;
                case 0x6:
                    /* SHR Vx {, Vy} */
                    v.at(0xF) = v.at(x) & 0x00000001;
                    v.at(x) = v.at(x) >>= 1;
                    pc = pc + 2;
                    break;
                case 0x7:
                    /* SUBN Vx, Vy */
                    v.at(0xF) = v.at(y) > v.at(x) ? 1 : 0;
                    v.at(x) = v.at(y) - v.at(x);
                    pc = pc + 2;
                    break;
                case 0xE:
                    /* SHL Vx {, Vy} */
                    v.at(0xF) = v.at(x) & 0x10000000;
                    v.at(x) = v.at(x) <<= 1;
                    pc = pc + 2;
                    break;
            }
            break;
        case 0x9:
            /* SNE Vx, Vy */
            if (v.at(x) != v.at(y))
                pc = pc + 4;
            else
                pc = pc + 2;
            break;
        case 0xA:
            /* LD I, addr */
            I = nnn;
            pc = pc + 2;
            break;
        case 0xB:
            /* JP V0, addr */
            pc = nnn + v.at(0x0);
            break;
        case 0xC:
            /* RND Vx, byte */
            v.at(x) = rnd() & kk;
            pc = pc + 2;
            break;
        case 0xD:
            /* DRW Vx, Vy, nibble */
            byte_t pixel;
            v.at(0xF) = 0x0;
            for (int yline = 0; yline < n; yline++) {
                for (int xline = 0; xline < 8; xline++) {
                    pixel = (memory.at(I + yline) & (0x80 >> xline)) != 0;
                    if (pixel) {
                        int idx =
                            (v.at(x) + xline) % 64 +
                            ((v.at(y) + yline) % 32) * 64;
                        if (display.at(idx)) {
                            v.at(0xF) = 0x1;
                            display.at(idx) = 0;
                        } else {
                            display.at(idx) = 1;
                        }
                    }
                }
            }
            drawFlag = 1;
            pc = pc + 2;
            break;
        case 0xE:
            switch (opcode & 0x00FF) {
                case 0x009E:
                    /* SKP Vx */
                    if (keys.at(v.at(x)))
                        pc = pc + 4;
                    else
                        pc = pc + 2;
                    break;
                case 0x00A1:
                    /* SKNP Vx */
                    if (!keys.at(v.at(x)))
                        pc = pc + 4;
                    else
                        pc = pc + 2;
                    break;
            }
            break;
        case 0xF:
            switch (opcode & 0x00FF) {
                case 0x0007:
                    /* LD Vx, DT */
                    v.at(x) = delayTimer;
                    pc = pc + 2;
                    break;
                case 0x000A: {
                    /* LD Vx, K */
                    bool keyPressed = false;
                    for (byte_t key : keys) {
                        if (key) {
                            keyPressed = true;
                            v.at(x) = key;
                        }
                    }
                    if (keyPressed)
                        pc = pc + 2;
                    break;
                }
                case 0x0015:
                    /* LD DT, Vx */
                    delayTimer = v.at(x);
                    pc = pc + 2;
                    break;
                case 0x0018:
                    /* LD ST, Vx */
                    soundTimer = v.at(x);
                    pc = pc + 2;
                    break;
                case 0x001E:
                    /* ADD I, Vx */
                    I = I + v.at(x);
                    pc = pc + 2;
                    break;
                case 0x0029:
                    /* LD F, Vx */
                    I = 0x50 + v.at(x) * 5;
                    pc = pc + 2;
                    break;
                case 0x0033:
                    /* LD B, Vx */
                    memory.at(I) = v.at(x) / 100;
                    memory.at(I + 1) = (v.at(x) / 10) % 10;
                    memory.at(I + 2) = v.at(x) % 10;
                    pc = pc + 2;
                    break;
                case 0x0055:
                    /* Fx55 - LD [I], Vx */
                    for (int idx = 0; idx <= x; idx++)
                        memory.at(I + idx) = v.at(idx);
                    I = I + x + 1; /* I is incremented in the specification */
                    pc = pc + 2;
                    break;
                case 0x0065:
                    /* LD Vx, [I] */
                    for (int idx = 0; idx <= x; idx++)
                        v.at(idx) = memory.at(I + idx);
                    I = I + x + 1; /* I is incremented in the specification */
                    pc = pc + 2;
                    break;
            }
            break;
    }
    /* When pc is more than 4095 begins to 0 again */
    pc = pc & 0xFFF;

    /* Update timers */
    if (delayTimer > 0)
        delayTimer--;
    if (soundTimer > 0) {
        if (soundTimer == 1) {
            /* TODO: BEEP */
            std::cout << "BEEP!!\n";
        }
        soundTimer--;
    }
    std::cout << "Delay Timer: " << (int) delayTimer << "\n";
    std::cout << "Sound Timer: " << (int) soundTimer << "\n";
}

void Machine::loadGame(std::ifstream& ifs) {
    ifs.seekg(0, std::ios::end);
    std::streamsize fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    /* Assert game size is valid */
    if (fileSize > (MEM_SIZE - MEM_PRG_START_POS)) {
        std::cerr << "Memory overflow while loading the ROM" << std::endl;
        exit(1);
    }
    /* Load all the file into memory */
    ifs.read(reinterpret_cast<char*>(&memory.at(MEM_PRG_START_POS)),
             fileSize);

    if (!ifs) {
        std::cerr << "Error reading ROM into memory" << std::endl;
    }
}

std::array<byte_t, 64 * 32>& Machine::getDisplay() {
    return display;
}

std::array<byte_t, 16>& Machine::getKeys() {
    return keys;
}

byte_t Machine::rnd() {
    return static_cast<byte_t>(rndDistrib(rndGen));
}
