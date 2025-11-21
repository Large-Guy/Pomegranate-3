#include "CopyPass.h"

#include "SDL3/SDL_gpu.h"

CopyPass::CopyPass(const std::shared_ptr<Renderer>& renderer) : RenderPass(renderer) {
}

void CopyPass::begin(const std::weak_ptr<CommandBuffer>& commandQueue) {
    if (commandQueue.expired()) {
        throw std::runtime_error("CommandQueue has expired");
    }
    std::shared_ptr<CommandBuffer> commandBuffer = commandQueue.lock();
    this->renderPass = SDL_BeginGPUCopyPass(static_cast<SDL_GPUCommandBuffer*>(commandBuffer->getInternal()));
}

void CopyPass::end() {
    if (renderPass == nullptr)
        return;
    SDL_EndGPUCopyPass(static_cast<SDL_GPUCopyPass*>(renderPass));
    renderPass = nullptr;
}

std::shared_ptr<CopyPass> CopyPass::make(const std::shared_ptr<Renderer>& renderer) {
    return std::shared_ptr<CopyPass>(new CopyPass(renderer));
}

