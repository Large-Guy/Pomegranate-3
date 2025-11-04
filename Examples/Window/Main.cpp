#include <iostream>
#include<Platform/Window.h>

#include "Core/App.h"
#include "SDL3/SDL_timer.h"

class WindowExample : public App {
public:
    Pointer<Window> window = Window::create();
    
    void start() override {
        window->setTitle("Window Example");
        window->open();
        
        window->setFlag(Window::Transparent, true);
        window->setFlag(Window::Blur, true);
        window->setFlag(Window::Resizable, true);
        window->setFlag(Window::Borderless, true);
        window->setFlag(Window::HighDPI, true);
        
        window->setBlurRadius(10);
    }

    void update(float delta) override {
    }
};

int main() {
    Pointer<WindowExample> app = new WindowExample();

    app->run();

    return 0;
}
