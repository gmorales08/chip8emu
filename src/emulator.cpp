#include "emulator.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

Emulator::Emulator(uint32_t _speed)
    : speed(_speed), graphics{machine.getBus()} {
  graphics.init();
}

void Emulator::readGame(const char* fileName) {
  std::ifstream ifs{fileName, std::ios_base::binary};
  if (!ifs.is_open()) {
    std::cerr << "Couldn't open file for reading" << std::endl;
    exit(1);
  }
  std::cout << "Game file readed" << std::endl;
  machine.loadGame(ifs);
}

void Emulator::run() {
  const double cycleTime = 1.0 / speed;         /* Seconds of a cycle */
  static constexpr auto screenUpdateRate = 60;  /* Hz */
  static constexpr auto speakerUpdateRate = 60; /* Hz */
  int cyclesSinceLastScreenUpdate = 0;
  int cyclesSinceLastSpeakerUpdate = 0;
  const int screenUpdateCycles = static_cast<int>(speed / screenUpdateRate);
  const int speakerUpdateCycles = static_cast<int>(speed / speakerUpdateRate);

  auto startTime = std::chrono::high_resolution_clock::now();

  static constexpr byte_t mustSound = 0x01;
  static constexpr byte_t mustDraw = 0x02;

  int cycles = 0;
  byte_t machineStatusReg = 0x0;
  while (!KeyboardController::shouldQuit()) {
    machine.cycle();

    cycles++;
    cyclesSinceLastScreenUpdate++;
    cyclesSinceLastSpeakerUpdate++;

    if (cyclesSinceLastScreenUpdate >= screenUpdateCycles) {
      machineStatusReg =
          machine.getBus().memoryReadByte(MachineConstants::statusRegMemAdr);
      if ((machineStatusReg & mustDraw) == 0x02) {
        graphics.render();
        /* The draw bit is reseted by the display controller */
      }
      cyclesSinceLastScreenUpdate = 0;
    }

    if (cyclesSinceLastSpeakerUpdate >= speakerUpdateCycles) {
      machineStatusReg =
          machine.getBus().memoryReadByte(MachineConstants::statusRegMemAdr);
      if ((machineStatusReg & mustSound) == 0x01) {
        sound.playBeep();
        /* The sound bit is reseted by the CPU */
      } else {
        sound.stopBeep();
      }
      cyclesSinceLastSpeakerUpdate = 0;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;
    if (elapsed.count() < cycleTime) {
      std::this_thread::sleep_for(
          std::chrono::duration<double>(cycleTime - elapsed.count()));
    }
    startTime = std::chrono::high_resolution_clock::now();
  }
}
