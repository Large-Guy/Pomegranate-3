#include "MacOsWindow.h"

#include <iostream>

#include "Effects.hpp"

void MacOSWindow::rebuild() {
    if (blur)
        destroyVibrancy(blur);
    StandardWindow::rebuild();
    if (flags[Blur])
        blur = createVibrancy(static_cast<SDL_Window*>(getInternal()), blurRadius);
}

void MacOSWindow::setBlurRadius(float intensity) {
    blurRadius = intensity;
    rebuild();
}