#include "memory.hpp"

Memory::Memory() : memory{} {}

void Memory::reset() { memory.fill(0x0); }

byte_t Memory::getByte(uint16_t address) { return memory.at(address); }

void Memory::setByte(uint16_t address, byte_t value) {
  memory.at(address) = value;
}
