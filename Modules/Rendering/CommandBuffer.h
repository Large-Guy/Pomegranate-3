#ifndef POMEGRANATE_COMMANDQUEUE_H
#define POMEGRANATE_COMMANDQUEUE_H
#include <memory>


class CommandBuffer {
public:
    explicit CommandBuffer(const std::shared_ptr<class Renderer>& device);

    ~CommandBuffer();

    void* getInternal();

private:
    std::shared_ptr<class Renderer> device;
    struct SDL_GPUCommandBuffer* commandBuffer;
};


#endif //POMEGRANATE_COMMANDQUEUE_H
