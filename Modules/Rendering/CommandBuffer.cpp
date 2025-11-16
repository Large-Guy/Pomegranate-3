#include "CommandBuffer.h"

#include "Renderer.h"
#include "SDL3/SDL_gpu.h"

CommandBuffer::CommandBuffer(const std::shared_ptr<Renderer>& device) {
    this->device = device;
    this->commandBuffer = SDL_AcquireGPUCommandBuffer(static_cast<SDL_GPUDevice*>(device->getInternal()));
}

CommandBuffer::~CommandBuffer() = default;

void* CommandBuffer::getInternal() {
    return commandBuffer;
}
