#ifndef POMEGRANATE_3_WINDOW_H
#define POMEGRANATE_3_WINDOW_H
#include <string>
#include <memory>

class Window : public std::enable_shared_from_this<Window> {
public:
    enum Flag {
        Transparent,
        Blur, //macOS, Windows
        Fullscreen,
        Resizable,
        Borderless,
        Full, //macOS only
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

    virtual void getSize(int& width, int& height) = 0;

    virtual void setPosition(int x, int y) = 0;

    virtual void getPosition(int& x, int& y) = 0;

    virtual void setTitle(std::string title) = 0;

    virtual std::string getTitle() = 0;

    virtual void setBlurRadius(float radius) = 0;

    virtual float getBlurRadius() = 0;

    virtual void display() = 0;

    virtual void* getInternal() = 0;

    static std::shared_ptr<Window> create();

protected:
    virtual void rebuild() = 0;
};


#endif //POMEGRANATE_3_WINDOW_H
