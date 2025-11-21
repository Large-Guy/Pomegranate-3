#ifndef POMEGRANATE_COMMANDQUEUE_H
#define POMEGRANATE_COMMANDQUEUE_H
#include <memory>


class CommandBuffer {
public:
    ~CommandBuffer();

    void* getInternal();

private:
    explicit CommandBuffer(const std::shared_ptr<class Renderer>& device);


    std::shared_ptr<class Renderer> device;
    struct SDL_GPUCommandBuffer* commandBuffer;

    friend class Renderer;
};


#endif //POMEGRANATE_COMMANDQUEUE_H
