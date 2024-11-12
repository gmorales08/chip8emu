#include "bus.hpp"

#include <stdexcept>

#include "cpu.hpp"

Bus::Bus() : cpu(new Cpu(*this)), mainMemory(new Memory()), auxiliaryMemory{} {}

byte_t Bus::memoryReadByte(uint16_t address) {
  if (address < MachineConstants::auxMemStartAdr) {
    return mainMemory->getByte(address);
  }
  if (address < MachineConstants::memAdrLimit) {
    return auxiliaryMemory.at(address - MachineConstants::auxMemStartAdr);
  }
  throw std::out_of_range("Memory address " + std::to_string(address) +
                          " is out of range: ");
}

void Bus::memoryWriteByte(uint16_t address, byte_t byte) {
  if (address < MachineConstants::auxMemStartAdr) {
    mainMemory->setByte(address, byte);
  } else if (address < MachineConstants::memAdrLimit) {
    auxiliaryMemory.at(address - MachineConstants::auxMemStartAdr) = byte;
  } else {
    throw std::out_of_range("Memory address " + std::to_string(address) +
                            " is out of range: ");
  }
}

void Bus::cycle() { cpu->cycle(); }

void Bus::reset() {
  /* Resets the CPU, main memory and auxiliar memory */
  cpu->reset();
  mainMemory->reset();
  for (auto i = 0; i < MachineConstants::auxMemSiz; i++) {
    auxiliaryMemory.at(i) = 0x0;
  }
}
