#include "MacOsWindow.h"

#include <iostream>

#include "Effects.hpp"

void MacOSWindow::rebuild() {
    if (blur)
        destroyVibrancy(blur);
    if (Full)
        flags[Borderless] = false;
    StandardWindow::rebuild();
    if (flags[Blur])
        blur = createVibrancy(window, blurRadius);
    if (flags[Full])
        setBorderlessWithTrafficLights(window);
}

void MacOSWindow::setBlurRadius(float intensity) {
    blurRadius = intensity;
    rebuild();
}

float MacOSWindow::getBlurRadius() {
    return blurRadius;
}
