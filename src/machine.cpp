#include "machine.hpp"

#include <iostream>

const std::array<byte_t, MachineConstants::fontsetMemSiz> Machine::fontset{
    0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
    0x20, 0x60, 0x20, 0x20, 0x70,  // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
    0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
    0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
    0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
    0xF0, 0x80, 0xF0, 0x80, 0x80   // F
};

Machine::Machine() { loadFontset(); }

void Machine::reset() {
  bus.reset();
  loadFontset();
}

void Machine::loadFontset() {
  /* Move the fontset to memory */
  for (auto i = 0; i < MachineConstants::fontsetMemSiz; i++) {
    bus.memoryWriteByte(MachineConstants::fontsetMemAdr + i, fontset.at(i));
  }
}

void Machine::loadGame(std::ifstream& ifs) {
  ifs.seekg(0, std::ios::end);
  std::streamsize fileSize = ifs.tellg();
  ifs.seekg(0, std::ios::beg);

  /* Assert game size is valid */
  if (fileSize > (Memory::memSize - MachineConstants::prgStartMemAdr)) {
    std::cerr << "Memory overflow while loading the ROM" << std::endl;
    exit(1);
  }
  /* Buffer for read file chars */
  std::vector<char> auxBuf(fileSize);
  /* Load all the file into the buffer */
  ifs.read(auxBuf.data(), fileSize);
  if (!ifs) {
    std::cerr << "Error reading ROM into buffer" << std::endl;
  }
  /* Load the buffer into memory */
  for (auto i = 0; i < fileSize; i++) {
    bus.memoryWriteByte(MachineConstants::prgStartMemAdr + i, auxBuf.at(i));
  }
}

void Machine::cycle() { bus.cycle(); }

Bus& Machine::getBus() { return bus; }
