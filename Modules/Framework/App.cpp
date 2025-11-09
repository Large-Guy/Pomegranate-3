#include "App.h"

#include <stdexcept>
#include <thread>

#include "Core/Pomegranate.h"
#include "SDL3/SDL_init.h"

void App::init() {
    Pomegranate::init();
    this->start();
}

App::~App() = default;

void App::process() {
    this->update(0.01f);
}

void App::run() {
    init();
    while (running) {
        process();
    }
}

void App::start() {
}

void App::update(float delta) {
}

void App::end() {
}

void App::quit() {
    this->running = false;
}

bool App::isRunning() const {
    return running;
}
