#include "Renderer.h"

#include <string>

#include "SDL3/SDL_gpu.h"

Renderer::Renderer() {
    device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_MSL, true, nullptr);
    if (device == nullptr)
        throw std::runtime_error(std::string("Failed to create GPU device ") + SDL_GetError());
}

void* Renderer::getInternal() const {
    return device;
}

std::weak_ptr<CommandBuffer> Renderer::begin() {
    currentCommandQueue = std::make_shared<CommandBuffer>(shared_from_this());
    return currentCommandQueue;
}

void Renderer::end() {
    if (!currentCommandQueue)
        return;
    const auto pSubmitFence = SDL_SubmitGPUCommandBufferAndAcquireFence(static_cast<SDL_GPUCommandBuffer*>(currentCommandQueue->getInternal()));
    SDL_WaitForGPUFences(device, true, &pSubmitFence, 1);
    SDL_ReleaseGPUFence(device, pSubmitFence);
    currentCommandQueue.reset();
}

std::weak_ptr<CommandBuffer> Renderer::getCommandQueue() {
    return currentCommandQueue;
}


