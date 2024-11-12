#include "sound.hpp"

#include <stdexcept>

void audioCallback(void* userdata, Uint8* stream, int len) {
  static int phase = 0;
  int frequency = 440;
  int sampleRate = 44100;
  int amplitude = 28000;

  int16_t* buffer = (int16_t*)stream;
  int samples = len / 2;

  for (auto i = 0; i < samples; i++) {
    buffer[i] = (phase < sampleRate / frequency / 2) ? amplitude : -amplitude;
    phase = (phase + 1) % (sampleRate / frequency);
  }
}

Sound::Sound() {
  if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    throw std::runtime_error("Failed to initialize SDL2 audio");
  }

  SDL_AudioSpec spec{};
  spec.freq = 44100;
  spec.format = AUDIO_S16SYS;
  spec.channels = 1;
  spec.samples = 2048;
  spec.callback = audioCallback;

  if (SDL_OpenAudio(&spec, nullptr) < 0) {
    throw std::runtime_error("Failed to open audio");
  }
}

Sound::~Sound() { SDL_CloseAudio(); }

void Sound::playBeep() { SDL_PauseAudio(0); }

void Sound::stopBeep() { SDL_PauseAudio(1); }
