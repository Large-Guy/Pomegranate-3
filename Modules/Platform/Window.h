#ifndef POMEGRANATE_3_WINDOW_H
#define POMEGRANATE_3_WINDOW_H
#include <Core/Pointer.h>
#include <string>

class Window
{
protected:
    virtual void rebuild() = 0;
public:
    enum Flag {
        Transparent,
        Blur,
        Fullscreen,
        Resizable,
        Borderless,
        HighDPI,
    };
    virtual ~Window();
    virtual void open() = 0;
    virtual void close() = 0;
    virtual void show() = 0;
    virtual void hide() = 0;
    virtual void setFlag(Flag flag, bool enable) = 0;
    virtual bool hasFlag(Flag flag) = 0;
    virtual void setSize(int width, int height) = 0;
    virtual void setPosition(int x, int y) = 0;
    virtual void setTitle(std::string title) = 0;
    virtual void setBlurRadius(float radius) = 0;

    virtual void display() = 0;

    virtual void* getInternal() = 0;
    
    static Pointer<Window> create();
};


#endif //POMEGRANATE_3_WINDOW_H