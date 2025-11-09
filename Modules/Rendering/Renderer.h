#ifndef POMEGRANATE_DEVICE_H
#define POMEGRANATE_DEVICE_H
#include "CommandQueue.h"

class Renderer : public std::enable_shared_from_this<Renderer> {
public:
    Renderer();
    std::weak_ptr<CommandQueue> begin();
    void end();
    std::weak_ptr<CommandQueue> getCommandQueue();
    void* getInternal() const;
private:
    struct SDL_GPUDevice* device;
    std::shared_ptr<CommandQueue> currentCommandQueue;
};


#endif //POMEGRANATE_DEVICE_H