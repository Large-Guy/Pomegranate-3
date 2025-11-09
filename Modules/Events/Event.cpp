#include "Event.h"

#include <stdexcept>
#include <string>

#include "SDL3/SDL.h"

static std::shared_ptr<Window> getWindowEventWin(const SDL_Event& e) {
    SDL_Window* sdlWindow = SDL_GetWindowFromID(e.window.windowID);
    const SDL_PropertiesID properties = SDL_GetWindowProperties(sdlWindow);
    return (static_cast<std::weak_ptr<Window>*>(SDL_GetPointerProperty(properties, "pomegranate", nullptr))->lock());
}

static std::shared_ptr<Window> getKeyEventWin(const SDL_Event& e) {
    SDL_Window* sdlWindow = SDL_GetWindowFromID(e.key.windowID);
    const SDL_PropertiesID properties = SDL_GetWindowProperties(sdlWindow);
    return (static_cast<std::weak_ptr<Window>*>(SDL_GetPointerProperty(properties, "pomegranate", nullptr))->lock());
}

bool Event::getEvent(Event& e) {
    SDL_Event event;
    if (!SDL_PollEvent(&event))
        return false;

    e.data = std::monostate();

    switch (event.type) {
        case SDL_EVENT_QUIT: {
            e.data = AppQuitEvent{
            };
            break;
        }
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED: {
            e.data = WindowCloseEvent{
                getWindowEventWin(event),
            };
            break;
        }
        case SDL_EVENT_WINDOW_RESIZED: {
            e.data = WindowResizeEvent{
                getWindowEventWin(event),
                event.window.data1,
                event.window.data2
            };
            break;
        }
        case SDL_EVENT_WINDOW_MOVED: {
            e.data = WindowMoveEvent{
                getWindowEventWin(event),
                event.window.data1,
                event.window.data2
            };
            break;
        }
        case SDL_EVENT_KEY_DOWN: {
            KeyPressEvent pressEvent;
            pressEvent.window = getKeyEventWin(event);
            pressEvent.scancode = static_cast<KeyEvent::ScanCode>(event.key.scancode);
            e.data = pressEvent;
            break;
        }
        case SDL_EVENT_KEY_UP: {
            KeyReleaseEvent releaseEvent;
            releaseEvent.window = getKeyEventWin(event);
            releaseEvent.scancode = static_cast<KeyEvent::ScanCode>(event.key.scancode);
            e.data = releaseEvent;
            break;
        }
        default: break;
    }

    return true;
}
