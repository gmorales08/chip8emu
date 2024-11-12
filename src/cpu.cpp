#include "cpu.hpp"

#include <iostream>

#include "machine.hpp"

Cpu::Cpu(Bus& bus)
    : bus{bus},
      keyboardController{bus},
      opcode{},
      stack{},
      v{},
      pc{MachineConstants::prgStartMemAdr},
      sp{},
      I{},
      delayTimer{},
      soundTimer{},
      cycleCount{},
      rndGen(std::random_device{}()),
      rndDistrib(rndMinNum, rndMaxNum),
      opcodeTable{&Cpu::OPCODE_NULL},
      opcodeTable0{&Cpu::OPCODE_NULL},
      opcodeTable8{&Cpu::OPCODE_NULL},
      opcodeTableE{&Cpu::OPCODE_NULL},
      opcodeTableF{&Cpu::OPCODE_NULL} {
  fillOpcodeTables();
}

void Cpu::reset() {
  opcode = 0x0000;
  stack.fill(0x0000);
  v.fill(0x00);
  pc = MachineConstants::prgStartMemAdr;
  sp = 0x0;
  I = 0x0000;
  cycleCount = 0x0;
  delayTimer = 0x00;
  soundTimer = 0x00;
}

void Cpu::cycle() {
  /* Fetch */
  opcode = static_cast<word_t>(bus.memoryReadByte(pc) << byteShift) |
           static_cast<word_t>(bus.memoryReadByte(pc + 1));
  /* Decode and Execute*/
  ((*this).*(opcodeTable.at(getHighByteHighNibble(opcode))))();

  /* When PC pass the limit return to the beggining */
  /* The begging in this interpreter is the 0x0 address */
  pc = pc % (MachineConstants::prgLimitMemAdr + 1);

  /* Update timers */
  static constexpr auto cyclesPerDelayTimerDecrement =
      MachineConstants::cpuSpeed / delayTimerDecrementSpeed;
  if (cycleCount % cyclesPerDelayTimerDecrement == 0 && delayTimer > 0) {
    delayTimer--;
  }

  static constexpr auto cyclesPerSoundTimerDecrement =
      MachineConstants::cpuSpeed / soundTimerDecrementSpeed;
  if (cycleCount % cyclesPerSoundTimerDecrement == 0) {
    if (soundTimer > 0) {
      setSoundBit();
      soundTimer--;

    } else {
      resetSoundBit();
    }
  }

  cycleCount++;
}

void Cpu::setSoundBit() {
  bus.memoryWriteByte(MachineConstants::statusRegMemAdr,
                      bus.memoryReadByte(MachineConstants::statusRegMemAdr) |
                          MachineConstants::setSoundMask);
}

void Cpu::resetSoundBit() {
  bus.memoryWriteByte(MachineConstants::statusRegMemAdr,
                      bus.memoryReadByte(MachineConstants::statusRegMemAdr) &
                          MachineConstants::resetSoundMask);
}

void Cpu::setDrawBit() {
  bus.memoryWriteByte(MachineConstants::statusRegMemAdr,
                      bus.memoryReadByte(MachineConstants::statusRegMemAdr) |
                          MachineConstants::setDrawMask);
}

byte_t Cpu::rnd() { return static_cast<byte_t>(rndDistrib(rndGen)); }

/* Opcode table functions */
void Cpu::handleOpcodeTable0() {
  ((*this).*(opcodeTable0.at(getLowByteLowNibble(opcode))))();
}

void Cpu::handleOpcodeTable8() {
  ((*this).*(opcodeTable8.at(getLowByteLowNibble(opcode))))();
}

void Cpu::handleOpcodeTableE() {
  ((*this).*(opcodeTableE.at(getLowByte(opcode))))();
}

void Cpu::handleOpcodeTableF() {
  ((*this).*(opcodeTableF.at(getLowByte(opcode))))();
}

void Cpu::fillOpcodeTables() {
  opcodeTable.at(opcodeTable0_idx) = &Cpu::handleOpcodeTable0;
  opcodeTable.at(JP_NNN_idx) = &Cpu::JP_NNN;
  opcodeTable.at(CALL_idx) = &Cpu::CALL;
  opcodeTable.at(SE_Vx_KK_idx) = &Cpu::SE_Vx_KK;
  opcodeTable.at(SNE_Vx_KK_idx) = &Cpu::SNE_Vx_KK;
  opcodeTable.at(SE_Vx_Vy_idx) = &Cpu::SE_Vx_Vy;
  opcodeTable.at(LD_Vx_KK_idx) = &Cpu::LD_Vx_KK;
  opcodeTable.at(ADD_Vx_KK_idx) = &Cpu::ADD_Vx_KK;
  opcodeTable.at(opcodeTable8_idx) = &Cpu::handleOpcodeTable8;
  opcodeTable.at(SNE_Vx_Vy_idx) = &Cpu::SNE_Vx_Vy;
  opcodeTable.at(LD_I_NNN_idx) = &Cpu::LD_I_NNN;
  opcodeTable.at(JP_V0_NNN_idx) = &Cpu::JP_V0_NNN;
  opcodeTable.at(RND_idx) = &Cpu::RND;
  opcodeTable.at(DRW_idx) = &Cpu::DRW;
  opcodeTable.at(opcodeTableE_idx) = &Cpu::handleOpcodeTableE;
  opcodeTable.at(opcodeTableF_idx) = &Cpu::handleOpcodeTableF;

  opcodeTable0.at(CLS_idx) = &Cpu::CLS;
  opcodeTable0.at(RET_idx) = &Cpu::RET;

  opcodeTable8.at(LD_Vx_Vy_idx) = &Cpu::LD_Vx_Vy;
  opcodeTable8.at(OR_idx) = &Cpu::OR;
  opcodeTable8.at(AND_idx) = &Cpu::AND;
  opcodeTable8.at(XOR_idx) = &Cpu::XOR;
  opcodeTable8.at(ADD_Vx_Vy_idx) = &Cpu::ADD_Vx_Vy;
  opcodeTable8.at(SUB_idx) = &Cpu::SUB;
  opcodeTable8.at(SHR_idx) = &Cpu::SHR;
  opcodeTable8.at(SUBN_idx) = &Cpu::SUBN;
  opcodeTable8.at(SHL_idx) = &Cpu::SHL;

  opcodeTableE.at(SKP_idx) = &Cpu::SKP;
  opcodeTableE.at(SKNP_idx) = &Cpu::SKNP;

  opcodeTableF.at(LD_Vx_DT_idx) = &Cpu::LD_Vx_DT;
  opcodeTableF.at(LD_Vx_KEY_idx) = &Cpu::LD_Vx_KEY;
  opcodeTableF.at(LD_DT_Vx_idx) = &Cpu::LD_DT_Vx;
  opcodeTableF.at(LD_ST_Vx_idx) = &Cpu::LD_ST_Vx;
  opcodeTableF.at(ADD_I_Vx_idx) = &Cpu::ADD_I_Vx;
  opcodeTableF.at(LD_I_FONT_idx) = &Cpu::LD_I_FONT;
  opcodeTableF.at(BCD_Vx_idx) = &Cpu::BCD_Vx;
  opcodeTableF.at(LD_I_Vx_idx) = &Cpu::LD_I_Vx;
  opcodeTableF.at(LD_Vx_I_idx) = &Cpu::LD_Vx_I;
}

/* Opcode byte operation functions */
constexpr byte_t Cpu::getLowByte(word_t opcode) { return opcode & lowByteMask; }

constexpr byte_t Cpu::getHighByteHighNibble(word_t opcode) {
  return static_cast<word_t>(opcode & highByteHighNibbleMask) >>
         byteAndNibbleShift;
}

constexpr byte_t Cpu::getHighByteLowNibble(word_t opcode) {
  return static_cast<word_t>(opcode & highByteLowNibbleMask) >> byteShift;
}

constexpr byte_t Cpu::getLowByteHighNibble(word_t opcode) {
  return opcode & lowByteHighNibbleMask;
}

constexpr byte_t Cpu::getLowByteLowNibble(word_t opcode) {
  return opcode & lowByteLowNibbleMask;
}

constexpr word_t Cpu::get_nnn(word_t opcode) { return opcode & nnnMask; }

constexpr byte_t Cpu::get_n(word_t opcode) { return opcode & nMask; }

constexpr byte_t Cpu::get_x(word_t opcode) {
  return static_cast<word_t>(opcode & xMask) >> byteShift;
}

constexpr byte_t Cpu::get_y(word_t opcode) {
  return static_cast<word_t>(opcode & yMask) >> nibbleShift;
}

constexpr byte_t Cpu::get_kk(word_t opcode) { return opcode & kkMask; }

/* Instruction functions */

void Cpu::CLS() {
  /* 00E0: CLS
   * Clears the display */
  for (auto i = 0; i < MachineConstants::displayMemSiz; i++) {
    bus.memoryWriteByte(MachineConstants::displayMemAdr + i, 0x0);
  }
  pc = pc + 2;
}

void Cpu::RET() {
  /* 00EE: RET
   * Return from a subroutine */
  pc = stack.at(sp);
  sp--;
  pc = pc + 2;
}

/*void Cpu::SYS() {*/
/*[> SYS addr <]*/
/*[> Not used by this interpreter <]*/
/*pc = pc + 2;*/
/*}*/

void Cpu::JP_NNN() {
  /* 1NNN: JP NNN
   * Jumps to NNN address */
  pc = get_nnn(opcode);
}

void Cpu::CALL() {
  /* 2NNN: CALL NNN
   * Calls a routine at NNN */
  sp++;
  stack.at(sp) = pc;
  pc = get_nnn(opcode);
}

void Cpu::SE_Vx_KK() {
  /* 3XKK: SE Vx, KK
   * Skips the next instruction if Vx == KK */
  if (v.at(get_x(opcode)) == get_kk(opcode)) {
    pc = pc + 4;
  } else {
    pc = pc + 2;
  }
}

void Cpu::SNE_Vx_KK() {
  /* 4XKK: SNE Vx, KK
   * Skips the next instruction if Vx != KK */
  if (v.at(get_x(opcode)) != get_kk(opcode)) {
    pc = pc + 4;
  } else {
    pc = pc + 2;
  }
}

void Cpu::SE_Vx_Vy() {
  /* 5XY0: SE Vx, Vy
   * Skips the next instruction if Vx == Vy */
  if (v.at(get_x(opcode)) == v.at(get_y(opcode))) {
    pc = pc + 4;
  } else {
    pc = pc + 2;
  }
}

void Cpu::LD_Vx_KK() {
  /* 6XKK: LD Vx, KK
   * Loads inmediate KK into Vx */
  v.at(get_x(opcode)) = get_kk(opcode);
  pc = pc + 2;
}

void Cpu::ADD_Vx_KK() {
  /* 7XKK: ADD Vx, KK
   * Adds inmediate KK to Vx. Does not affect Vf */
  v.at(get_x(opcode)) = v.at(get_x(opcode)) + get_kk(opcode);
  pc = pc + 2;
}

void Cpu::LD_Vx_Vy() {
  /* 8XY0: LD Vx, Vy
   * Loads the value in Vy into Vx */
  v.at(get_x(opcode)) = v.at(get_y(opcode));
  pc = pc + 2;
}

void Cpu::OR() {
  /* 8XY1: OR Vx, Vy
   * Sets Vx to (Vx OR Vy) */
  v.at(get_x(opcode)) = v.at(get_x(opcode)) | v.at(get_y(opcode));
  v.at(VF_idx) = 0x0; /* COSMAC VIP quirk */
  pc = pc + 2;
}

void Cpu::AND() {
  /* 8XY2: AND Vx, Vy
   * Sets Vx to (Vx AND Vy) */
  v.at(get_x(opcode)) = v.at(get_x(opcode)) & v.at(get_y(opcode));
  v.at(VF_idx) = 0x0; /* COSMAC VIP quirk */
  pc = pc + 2;
}

void Cpu::XOR() {
  /* 8XY3: XOR Vx, Vy
   * Sets Vx to (Vx XOR Vy) */
  v.at(get_x(opcode)) = v.at(get_x(opcode)) ^ v.at(get_y(opcode));
  v.at(VF_idx) = 0x0; /* COSMAC VIP quirk */
  pc = pc + 2;
}

void Cpu::ADD_Vx_Vy() {
  /* 8XY4: ADD Vx, Vy
   * Adds Vy to Vx. Vf is set if there is an overflow or reset if not */
  static constexpr auto maxByteValue = 255;
  word_t sum = v.at(get_x(opcode)) + v.at(get_y(opcode));
  v.at(get_x(opcode)) = getLowByte(sum);
  v.at(VF_idx) = sum > maxByteValue ? 1 : 0;
  pc = pc + 2;
}

void Cpu::SUB() {
  /* 8XY5: SUB Vx, Vy
   * Substracts Vy from Vx. Vf is reset when there is an underflow or set when
   * there is not */
  bool borrow = v.at(get_x(opcode)) < v.at(get_y(opcode));
  v.at(get_x(opcode)) = v.at(get_x(opcode)) - v.at(get_y(opcode));
  if (borrow) {
    v.at(VF_idx) = 0;
  } else {
    v.at(VF_idx) = 1;
  }
  pc = pc + 2;
}

void Cpu::SHR() {
  /* 8XY6: SHR Vx ,Vy
   * Vx is right shifted 1 bit. Vf is set to the least significant bit of Vx
   * prior to the shift. Original interpreter sets Vx with the value of Vy
   * bitshifted right 1, but modern interpreters ignore Vy */
  static constexpr auto lowBitMask = 0x01U;
  auto lowBitOfVx = v.at(get_x(opcode)) & lowBitMask;
  v.at(get_x(opcode)) = v.at(get_y(opcode)) >>= 1U;
  v.at(VF_idx) = lowBitOfVx;
  pc = pc + 2;
}

void Cpu::SUBN() {
  /* 8XY7: SUBN Vx, Vy
   * Sets Vx to Vy minus Vx. Vf is reset when there's an underflow and set when
   * there is not */
  bool borrow = v.at(get_y(opcode)) < v.at(get_x(opcode));
  v.at(get_x(opcode)) = v.at(get_y(opcode)) - v.at(get_x(opcode));
  if (borrow) {
    v.at(VF_idx) = 0;
  } else {
    v.at(VF_idx) = 1;
  }
  pc = pc + 2;
}

void Cpu::SHL() {
  /* 8XYE: SHL Vx, Vy
   * Vx is left shifted 1 bit. Vf is set to most significant bit of Vx prior to
   * the shift. Original interpreter sets Vx with the value of Vy bitshifted
   * left 1, but modern interpreters ignore Vy */
  static constexpr auto highBitMask = 0x80U;
  static constexpr auto highBitShift = 7U;
  auto highBitOfVx = (v.at(get_x(opcode)) & highBitMask) >> highBitShift;
  v.at(get_x(opcode)) = v.at(get_y(opcode)) <<= 1U;
  v.at(VF_idx) = highBitOfVx;
  pc = pc + 2;
}

void Cpu::SNE_Vx_Vy() {
  /* 9XY0: SNE Vx, Vy
   * Skips the next instruction if Vx != Vy */
  if (v.at(get_x(opcode)) != v.at(get_y(opcode))) {
    pc = pc + 4;
  } else {
    pc = pc + 2;
  }
}

void Cpu::LD_I_NNN() {
  /* ANNN: LD I, NNN
   * Sets I to NNN */
  I = get_nnn(opcode);
  pc = pc + 2;
}

void Cpu::JP_V0_NNN() {
  /* BNNN: JP V0, NNN
   * Sets PC to (V0 + NNN) */
  pc = get_nnn(opcode) + v.at(V0_idx);
}

void Cpu::RND() {
  /* CXKK: RND Vx, KK
   * Sets Vx to (rnd() & KK). rnd() generates a random between 0 and 255 */
  v.at(get_x(opcode)) = rnd() & get_kk(opcode);
  pc = pc + 2;
}

void Cpu::DRW() {
  /* DXYN: DRW Vx, Vy, N
   * Draws a sprite at coordinate (Vx, Vy) that has a width of 8 pixels and a
   * height of N pixels. Each row of 8 pixels is read as bit-coded starting from
   * memory location I. I value does not change after the instruction. Vf is set
   * if any pixel are flipped from 1 to 0. */
  static constexpr auto bytesPerRow = 8U;

  bool collision = false;
  v.at(VF_idx) = 0x0;

  /* Each file of the sprite is stored in 1 byte (8 pixels, 1 per bit) */
  for (auto spriteRowIdx = 0U; spriteRowIdx < get_n(opcode); spriteRowIdx++) {
    byte_t currentRow = bus.memoryReadByte(I + spriteRowIdx);

    auto xIdx = v.at(get_x(opcode));
    auto yIdx = v.at(get_y(opcode)) + spriteRowIdx;
    auto displayMemXIdx = xIdx % MachineConstants::displayWidth;
    auto displayMemYIdx = yIdx % MachineConstants::displayHeight;

    /* The display memory stores 8 bytes per row (64 pixels in a row) 32 times
     * (32 rows in the display). Display Memory = 8 * 32 = 256 bytes */
    auto displayMemByteIdx = MachineConstants::displayMemAdr +
                             ((displayMemYIdx)*bytesPerRow) +
                             ((displayMemXIdx) / bytesPerRow);
    auto displayMemBitOffset = displayMemXIdx % bytesPerRow;

    /* The pixels are XORed with the previous */
    byte_t displayPreviousByte = bus.memoryReadByte(displayMemByteIdx);
    byte_t byteNewValue =
        (currentRow >> displayMemBitOffset) ^ displayPreviousByte;
    /* Check collision */
    if ((displayPreviousByte & (currentRow >> displayMemBitOffset)) != 0) {
      collision = true;
    }
    bus.memoryWriteByte(displayMemByteIdx, byteNewValue);

    /* If the row takes 2 bytes */
    if (displayMemBitOffset != 0) {
      /* If the first byte is the last byte of the display memory, the second
       * will be the first byte */
      displayMemByteIdx++;
      if (displayMemByteIdx >= MachineConstants::displayMemAdrLimit) {
        displayMemByteIdx = MachineConstants::displayMemAdr;
      }

      displayPreviousByte = bus.memoryReadByte(displayMemByteIdx);
      byteNewValue =
          (currentRow << (8 - displayMemBitOffset)) ^ displayPreviousByte;
      /* Check collision */
      if ((displayPreviousByte & (currentRow << (8 - displayMemBitOffset))) !=
          0) {
        collision = true;
      }
      bus.memoryWriteByte(displayMemByteIdx, byteNewValue);
    }
  }

  if (collision) {
    v.at(VF_idx) = 0x1;
  }
  setDrawBit();
  pc = pc + 2;
}

void Cpu::SKP() {
  /* EX9E: SKP Vx
   * Skips the next instruction if the key stored in Vx is pressed */

  /* For know if the key is in the first or in the second IO register */
  static constexpr auto byteDivisor = 8;
  keyboardController.scanKeyboard();
  byte_t keyIdx = v.at(get_x(opcode));
  auto ioRegAdr = MachineConstants::ioRegsMemAdr + (keyIdx / byteDivisor);

  if ((bus.memoryReadByte(ioRegAdr) & MachineConstants::keyMasks.at(keyIdx)) !=
      0) {
    pc = pc + 4;
  } else {
    pc = pc + 2;
  }
}

void Cpu::SKNP() {
  /* EXA1: SKNP Vx
   * Skips the next instruction if the key stored in Vx is not pressed */

  /* For know if the key is in the first or in the second IO register */
  static constexpr auto byteDivisor = 8;
  keyboardController.scanKeyboard();
  byte_t keyIdx = v.at(get_x(opcode));
  auto ioRegAdr = MachineConstants::ioRegsMemAdr + (keyIdx / byteDivisor);

  if ((bus.memoryReadByte(ioRegAdr) & MachineConstants::keyMasks.at(keyIdx)) ==
      0) {
    pc = pc + 4;
  } else {
    pc = pc + 2;
  }
}

void Cpu::LD_Vx_DT() {
  /* FX07: LD Vx, DT
   * Sets Vx to the value of delay timer */
  v.at(get_x(opcode)) = delayTimer;
  pc = pc + 2;
}

void Cpu::LD_Vx_KEY() {
  /* FX0A: LD Vx, KEY
   * Waits for a key press an store its value in Vx */

  /* For know if the key is in the first or in the second IO register */
  static constexpr auto secondByte = 8;

  keyboardController.scanKeyboard();
  /* Check if the key was pressed. If so the instruction must wait for the
   * release of the key */
  static int keyPressed = -1;
  byte_t ioReg1Val = bus.memoryReadByte(MachineConstants::ioRegsMemAdr);
  byte_t ioReg2Val = bus.memoryReadByte(MachineConstants::ioRegsMemAdr + 1);
  /* Checks if a key is pressed */
  if (keyPressed == -1) {
    for (auto keyIdx = 0; keyIdx < MachineConstants::numOfKeys; keyIdx++) {
      byte_t ioRegVal = (keyIdx < secondByte) ? ioReg1Val : ioReg2Val;
      if ((ioRegVal & MachineConstants::keyMasks.at(keyIdx)) != 0) {
        keyPressed = keyIdx;
        break;
      }
    }
  }
  /* Check if the key is released */
  if (keyPressed != -1) {
    byte_t ioRegVal = (keyPressed < secondByte) ? ioReg1Val : ioReg2Val;
    if ((ioRegVal & MachineConstants::keyMasks.at(keyPressed)) == 0) {
      v.at(get_x(opcode)) = keyPressed;
      keyPressed = -1;
      pc = pc + 2;
    }
  }
}

void Cpu::LD_DT_Vx() {
  /* FX15: LD DT, Vx
   * Sets the delay timer to Vx */
  delayTimer = v.at(get_x(opcode));
  pc = pc + 2;
}

void Cpu::LD_ST_Vx() {
  /* FX18: LD ST, Vx
   * Sets the sound timer to Vx */
  soundTimer = v.at(get_x(opcode));
  pc = pc + 2;
}

void Cpu::ADD_I_Vx() {
  /* FX1E: ADD I, Vx
   * Sets I to (I + Vx). Vf is not affected */
  I = I + v.at(get_x(opcode));
  pc = pc + 2;
}

void Cpu::LD_I_FONT() {
  /* FX29: LD I, FONT(Vx)
   * Sets I to the location of the sprite for the character in Vx */
  I = MachineConstants::fontsetMemAdr +
      v.at(get_x(opcode)) * MachineConstants::fontCharHeight;
  pc = pc + 2;
}

void Cpu::BCD_Vx() {
  /* FX33: BCD Vx
   * Stores the BCD representation of Vx in I, I+1 and I+2 for the hundreds,
   * tens and ones respectively */
  static constexpr auto hundredsDivisor = 100;
  static constexpr auto tensDivisor = 10;
  static constexpr auto digitMod = 10;

  bus.memoryWriteByte(I, (v.at(get_x(opcode)) / hundredsDivisor));
  bus.memoryWriteByte(I + 1, (v.at(get_x(opcode)) / tensDivisor) % digitMod);
  bus.memoryWriteByte(I + 2, v.at(get_x(opcode)) % digitMod);
  pc = pc + 2;
}

void Cpu::LD_I_Vx() {
  /* Fx55: LD [I], Vx
   * Stores from V0 to Vx in the range (I, I + x) */
  for (int idx = 0; idx <= get_x(opcode); idx++) {
    bus.memoryWriteByte(I + idx, v.at(idx));
  }
  /* I is incremented in the specification */
  I = I + get_x(opcode) + 1;
  pc = pc + 2;
}

void Cpu::LD_Vx_I() {
  /* FX65: LD Vx, [I]
   * Loads from the range (I, I + x) into V0 to Vx */
  for (int idx = 0; idx <= get_x(opcode); idx++) {
    v.at(idx) = bus.memoryReadByte(I + idx);
  }
  /* I is incremented in the specification */
  I = I + get_x(opcode) + 1;
  pc = pc + 2;
}

void Cpu::OPCODE_NULL() { /* OPCODE not recognised */
}
