#ifndef POMEGRANATE_STANDARDWINDOW_H
#define POMEGRANATE_STANDARDWINDOW_H
#include "Core/BitFlag.h"
#include "Platform/Window.h"
#include "SDL3/SDL.h"


class StandardWindow : public Window {
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

    void getSize(int& w, int& h) override;

    void getRealSize(int& w, int& h) override;

    void setPosition(int x, int y) override;

    void getPosition(int& x, int& y) override;

    float getScale() override;

    void setTitle(std::string title) override;

    std::string getTitle() override;

    void setBlurRadius(float radius) override;

    float getBlurRadius() override;

    void display() override;

    void* getInternal() override;

    void* getInternalRenderer();

protected:
    void rebuild() override;

    BitFlag<Flag> flags;

    SDL_Window* window;

private:
    std::string title;
    int width, height;
};


#endif //POMEGRANATE_STANDARDWINDOW_H
