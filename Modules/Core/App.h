#ifndef POMEGRANATE_APP_H
#define POMEGRANATE_APP_H
#include <thread>


class App {
    bool running = true;
    void init();
    void process();
public:
    void run();

    virtual ~App();
    
    virtual void start();
    virtual void update(float delta);
    virtual void end();

    void quit();
    bool isRunning() const;
};


#endif //POMEGRANATE_APP_H