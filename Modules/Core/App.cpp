#include "App.h"

#include <stdexcept>
#include <thread>

#include "SDL3/SDL_init.h"

void App::init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error("Failed to initialize SDL: " + std::string(SDL_GetError()));
    }
    this->start();
}

App::~App() = default;

void App::process() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                running = false;
        }
    }

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
