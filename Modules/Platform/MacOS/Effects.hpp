#ifndef SHIFTY_VIBRANCY_HPP
#define SHIFTY_VIBRANCY_HPP
#include "SDL3/SDL_video.h"

void* createVibrancy(SDL_Window* window, float intensity);
void destroyVibrancy(void* blur);

#endif //SHIFTY_VIBRANCY_HPP
