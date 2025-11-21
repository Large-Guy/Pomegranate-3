#ifndef POMEGRANATE_DEVICE_H
#define POMEGRANATE_DEVICE_H
#include "CommandBuffer.h"

class Renderer : public std::enable_shared_from_this<Renderer> {
public:
    std::weak_ptr<CommandBuffer> begin();

    void end();

    std::weak_ptr<CommandBuffer> getCommandQueue();

    void* getInternal() const;

    static std::shared_ptr<Renderer> make();

private:
    Renderer();

    struct SDL_GPUDevice* device;
    std::shared_ptr<CommandBuffer> currentCommandQueue;
};


#endif //POMEGRANATE_DEVICE_H
