#ifndef BUS_HPP
#define BUS_HPP

#include <memory>

class Cpu;
#include "machineConstants.hpp"
#include "memory.hpp"

class Bus {
 public:
  Bus();
  byte_t memoryReadByte(uint16_t address);
  void memoryWriteByte(uint16_t address, byte_t byte);

  void cycle();
  void reset();

 private:
  Cpu* cpu;
  std::unique_ptr<Memory> mainMemory;
  std::array<byte_t, MachineConstants::auxMemSiz> auxiliaryMemory;
};

#endif /* BUS_HPP */
