#include "Window.h"
#ifdef __APPLE__

#include "MacOS/MacOsWindow.h"

#elif __linux__

#include "Linux/LinuxWindow"

#elif WIN32

#include "Windows/WindowsWindow"

#endif

Window::~Window() = default;


std::shared_ptr<Window> Window::create() {
    std::shared_ptr<Window> window = nullptr;
#ifdef __APPLE__
    window = std::make_shared<MacOSWindow>();
#elif __linux__
    window = std::make_shared<WindowsWindow>();
#elif WIN32
    window = std::make_shared<LinuxWindow>();
#endif

    return window;
}
