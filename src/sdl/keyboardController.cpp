#include "keyboardController.hpp"

const std::array<uint8_t, KeyboardController::keyMapSize>
    KeyboardController::keyMap = {
        SDLK_x, SDLK_1, SDLK_2, SDLK_3, SDLK_q, SDLK_w, SDLK_e, SDLK_a,
        SDLK_s, SDLK_d, SDLK_z, SDLK_c, SDLK_4, SDLK_r, SDLK_f, SDLK_v};

KeyboardController::KeyboardController(Bus& bus) : bus{bus}, quit{false} {}

void KeyboardController::scanKeyboard() {
  static constexpr auto firstReg = 8U;
  byte_t ioReg1 = 0x00;
  byte_t ioReg2 = 0x00;
  const Uint8* keyState = SDL_GetKeyboardState(nullptr);

  for (byte_t i = 0; i < keyMapSize; ++i) {
    byte_t isPressed =
        (keyState[SDL_GetScancodeFromKey(keyMap[i])] != 0) ? 1 : 0;
    if (i < firstReg) {
      ioReg1 |= (isPressed * MachineConstants::keyMasks.at(i));
    } else {
      ioReg2 |= (isPressed * MachineConstants::keyMasks.at(i));
    }
  }

  bus.memoryWriteByte(MachineConstants::ioRegsMemAdr, ioReg1);
  bus.memoryWriteByte(MachineConstants::ioRegsMemAdr + 1, ioReg2);
}

bool KeyboardController::shouldQuit() {
  SDL_Event event;

  return (SDL_PollEvent(&event) != 0) && (event.type == SDL_QUIT);
}
