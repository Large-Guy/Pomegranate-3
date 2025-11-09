#include "Pomegranate.h"

#include <stdexcept>
#include <string>

#include "SDL3/SDL.h"

void Pomegranate::init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error("Failed to initialize SDL: " + std::string(SDL_GetError()));
    }
}

