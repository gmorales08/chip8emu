#include <stdexcept>
#include "sound.hpp"

Sound::Sound() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        throw std::runtime_error("Failed to initialize SDL2 audio");
    }

    SDL_AudioSpec spec {};
    spec.freq = 44100;
    spec.format = AUDIO_S16SYS;
    spec.channels = 1;
    spec.samples = 2048;
    spec.callback = nullptr;

    if (SDL_OpenAudio(&spec, nullptr) < 0) {
        throw std::runtime_error("Failed to open audio");
    }
}

Sound::~Sound() {
    SDL_CloseAudio();
}

void Sound::playBeep() {
    SDL_PauseAudio(0);
}

void Sound::stopBeep() {
    SDL_PauseAudio(1);
}