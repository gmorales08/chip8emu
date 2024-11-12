#include "emulator.hpp"

int main(int argc, char** argv) {
  Emulator emulator(MachineConstants::cpuSpeed);
  if (argc == 2) {
    emulator.readGame(argv[1]);
  } else {
    exit(0);
  }
  emulator.run();

  return 0;
}
