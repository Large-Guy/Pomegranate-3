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
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> delta = now - lastUpdate;

    this->update(static_cast<float>(delta.count()));
    lastUpdate = now;
}

void App::run() {
    init();
    lastUpdate = std::chrono::high_resolution_clock::now();
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
