#include "Window.h"
#ifdef __APPLE__

#include "MacOS/MacOsWindow.h"

#elif __linux__

#include "Linux/LinuxWindow"

#elif WIN32

#include "Windows/WindowsWindow"

#endif

Window::~Window() = default;


Pointer<Window> Window::create() {
    Pointer<Window> window = nullptr;
#ifdef __APPLE__
    window = newPtr<MacOSWindow>();
#elif __linux__
    window = newPtr<WindowsWindow>();
#elif WIN32
    window = newPtr<LinuxWindow>();
#endif
    
    return window;
}
