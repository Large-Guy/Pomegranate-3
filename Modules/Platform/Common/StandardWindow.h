#ifndef POMEGRANATE_STANDARDWINDOW_H
#define POMEGRANATE_STANDARDWINDOW_H
#include "Core/BitFlag.h"
#include "Platform/Window.h"
#include "SDL3/SDL.h"


class StandardWindow : public Window {
    std::string title;
    int width, height;
    SDL_Window* window;
protected:
    void rebuild() override;
    BitFlag<Flag> flags;
public:
    StandardWindow();
    ~StandardWindow() override;

    void open() override;

    void close() override;
    
    void show() override;

    void hide() override;

    void setFlag(Flag flag, bool enable) override;

    bool hasFlag(Flag flag) override;

    void setSize(int w, int h) override;

    void setPosition(int x, int y) override;

    void setTitle(std::string title) override;

    void setBlurRadius(float radius) override;

    void display() override;

    void* getInternal() override;
};


#endif //POMEGRANATE_STANDARDWINDOW_H