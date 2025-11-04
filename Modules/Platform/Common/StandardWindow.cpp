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
}

void StandardWindow::close() {
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