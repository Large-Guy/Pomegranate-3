#ifndef POMEGRANATE_COMMANDQUEUE_H
#define POMEGRANATE_COMMANDQUEUE_H
#include <memory>


class CommandQueue {
public:
    CommandQueue(const std::shared_ptr<class Renderer>& device);
    ~CommandQueue();

    void* getInternal();
private:
    std::shared_ptr<class Renderer> device;
    struct SDL_GPUCommandBuffer* commandBuffer;
    
};


#endif //POMEGRANATE_COMMANDQUEUE_H