#include "Buffer.h"

#include <stdexcept>

#include "SDL3/SDL_gpu.h"

Buffer::Buffer(const std::shared_ptr<Renderer>& renderer, Type usage, size_t size) {
    this->renderer = renderer;
    this->size = size;
    this->usage = usage;
    SDL_GPUBufferCreateInfo info{};
    SDL_GPUBufferUsageFlags flags = 0;
    switch (usage) {
        case Type::Vertex:
            flags = SDL_GPU_BUFFERUSAGE_VERTEX;
            break;
        case Type::Index:
            flags = SDL_GPU_BUFFERUSAGE_INDEX;
            break;
        default:
            throw std::runtime_error("Invalid usage");
    }
    info.usage = flags;
    info.size = size;
    this->buffer = SDL_CreateGPUBuffer(static_cast<SDL_GPUDevice*>(renderer->getInternal()), &info);
}

Buffer::~Buffer() {
    SDL_ReleaseGPUBuffer(static_cast<SDL_GPUDevice*>(renderer->getInternal()), this->buffer);
}

Buffer::Type Buffer::type() {
    return usage;
}
