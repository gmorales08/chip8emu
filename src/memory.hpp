#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <array>

#include "types.hpp"

class Memory {
 public:
  /* Size of the machine memory (in bytes) */
  static constexpr auto memSize = 4096;

  Memory();
  void reset();
  byte_t getByte(uint16_t address);
  void setByte(uint16_t address, byte_t value);

 private:
  std::array<byte_t, memSize> memory;
};

#endif /* MEMORY_HPP */
