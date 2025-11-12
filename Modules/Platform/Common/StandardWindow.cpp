#include "StandardWindow.h"

void StandardWindow::rebuild() {
    close();
    open();
}

StandardWindow::StandardWindow() {
    this->title = "";
    this->width = 800;
    this->height = 600;
    this->window = nullptr;
}

StandardWindow::~StandardWindow() {
    SDL_DestroyWindow(this->window);
}

void StandardWindow::open() {
    SDL_WindowFlags windowFlags = 0;
    if (flags[Transparent])
        windowFlags |= SDL_WINDOW_TRANSPARENT;
    if (flags[Resizable])
        windowFlags |= SDL_WINDOW_RESIZABLE;
    if (flags[Fullscreen])
        windowFlags |= SDL_WINDOW_FULLSCREEN;
    if (flags[Borderless])
        windowFlags |= SDL_WINDOW_BORDERLESS;
    if (flags[HighDPI])
        windowFlags |= SDL_WINDOW_HIGH_PIXEL_DENSITY;
    this->window = SDL_CreateWindow(title.c_str(), width, height, windowFlags);

    auto* weak = new std::weak_ptr<Window>(shared_from_this());
    SDL_SetPointerProperty(SDL_GetWindowProperties(window), "pomegranate", weak);
}

void StandardWindow::close() {
    const SDL_PropertiesID properties = SDL_GetWindowProperties(window);
    const auto* weak = static_cast<std::weak_ptr<Window>*>(SDL_GetPointerProperty(properties, "pomegranate", nullptr));
    delete weak;
    SDL_SetPointerProperty(properties, "pomegranate", nullptr);
    SDL_DestroyWindow(this->window);
    this->window = nullptr;
}

void StandardWindow::show() {
    SDL_ShowWindow(this->window);
}

void StandardWindow::hide() {
    SDL_HideWindow(this->window);
}

void StandardWindow::setFlag(Flag flag, bool enable) {
    flags[flag] = enable;
    rebuild();
}

bool StandardWindow::hasFlag(Flag flag) {
    return flags[flag];
}

void StandardWindow::setSize(int w, int h) {
    this->width = w;
    this->height = h;
    if (!this->window)
        return;
    SDL_SetWindowSize(this->window, this->width, this->height);
}

void StandardWindow::getSize(int& w, int& h) {
    SDL_GetWindowSize(this->window, &w, &h);
    this->width = w;
    this->height = h;
}

void StandardWindow::getRealSize(int& w, int& h) {
    SDL_GetWindowSizeInPixels(window, &w, &h);
}

void StandardWindow::getPosition(int& x, int& y) {
    SDL_GetWindowPosition(this->window, &x, &y);
}

std::string StandardWindow::getTitle() {
    return this->title;
}

float StandardWindow::getBlurRadius() {
    return 0.0f;
}

void StandardWindow::setPosition(int x, int y) {
    if (!this->window)
        return;
    SDL_SetWindowPosition(this->window, x, y);
}

void StandardWindow::setTitle(std::string title) {
    this->title = title;
    if (!this->window)
        return;

    SDL_SetWindowTitle(this->window, title.c_str());
}

void StandardWindow::setBlurRadius(float radius) {
    //This isn't available on all platforms
}

void StandardWindow::display() {
}

void* StandardWindow::getInternal() {
    return this->window;
}

void* StandardWindow::getInternalRenderer() {
    return this->window;
}
