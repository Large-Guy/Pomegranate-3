#ifndef POMEGRANATE_APP_H
#define POMEGRANATE_APP_H
#include <thread>


class App {
public:
    void run();

    virtual ~App();

    virtual void start();

    virtual void update(float delta);

    virtual void end();

    void quit();

    bool isRunning() const;

private:
    std::chrono::time_point<std::chrono::steady_clock> lastUpdate;
    bool running = true;

    void init();

    void process();
};


#endif //POMEGRANATE_APP_H
