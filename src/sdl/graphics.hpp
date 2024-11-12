#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <SDL2/SDL.h>

#include <array>

#include "../bus.hpp"
#include "../machineConstants.hpp"
#include "../types.hpp"

class Graphics {
 public:
  Graphics(Bus& bus);
  ~Graphics();

  Graphics(const Graphics&) = delete;
  Graphics& operator=(const Graphics&) = delete;
  Graphics(Graphics&&) = delete;
  Graphics& operator=(Graphics&&) = delete;

  void init();
  void render();
  void resetDrawBit();

 private:
  Bus& bus;
  static constexpr auto scaleFactor = 10;
  static constexpr auto windowWidth =
      MachineConstants::displayWidth * scaleFactor;
  static constexpr auto windowHeight =
      MachineConstants::displayHeight * scaleFactor;

  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* texture;
};

#endif /* GRAPHICS_HPP */
