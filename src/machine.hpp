#ifndef MACHINE_HPP
#define MACHINE_HPP

#include <array>
#include <fstream>
#include <random>

#include "bus.hpp"
#include "machineConstants.hpp"
#include "types.hpp"

class Machine {
 public:
  Machine();
  void reset();
  /* Loads the game to memory. Needs the pointer to the game file
   * on disk */
  void loadGame(std::ifstream& ifs);

  void cycle();

  Bus& getBus();

 private:
  Bus bus;
  /* Constants */
  static const std::array<byte_t, MachineConstants::fontsetMemSiz> fontset;

  void loadFontset();

  // std::array<byte_t, MachineConstants::numOfKeys> keys; [> Key status <]
};

#endif
