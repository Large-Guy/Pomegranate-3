#include "CommandQueue.h"

#include "Renderer.h"
#include "SDL3/SDL_gpu.h"

CommandQueue::CommandQueue(const std::shared_ptr<Renderer>& device) {
    this->device = device;
    this->commandBuffer = SDL_AcquireGPUCommandBuffer(static_cast<SDL_GPUDevice*>(device->getInternal()));
}

CommandQueue::~CommandQueue() = default;

void* CommandQueue::getInternal() {
    return commandBuffer;
}
