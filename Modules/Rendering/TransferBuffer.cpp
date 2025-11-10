#include "TransferBuffer.h"

#include <stdexcept>

#include "SDL3/SDL_gpu.h"

TransferBuffer::TransferBuffer(std::shared_ptr<Renderer> renderer, Type type, size_t size) {
    this->renderer = renderer;
    this->bufferSize = size;

    SDL_GPUTransferBufferUsage flags = {};
    switch (type) {
        case Type::Download:
            flags = SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD;
            break;
        case Type::Upload:
            flags = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
            break;
        default:
            throw std::runtime_error("Invalid usage");
    }

    SDL_GPUTransferBufferCreateInfo info{};
    info.usage = flags;
    info.size = size;
    this->buffer = SDL_CreateGPUTransferBuffer(static_cast<SDL_GPUDevice*>(renderer->getInternal()), &info);
}

TransferBuffer::~TransferBuffer() {
    //Clear
    SDL_ReleaseGPUTransferBuffer(static_cast<SDL_GPUDevice*>(renderer->getInternal()), buffer);
}

void* TransferBuffer::mapPtr() const {
    return SDL_MapGPUTransferBuffer(static_cast<SDL_GPUDevice*>(renderer->getInternal()), buffer, false);
}

void TransferBuffer::unmap() const {
    SDL_UnmapGPUTransferBuffer(static_cast<SDL_GPUDevice*>(renderer->getInternal()), buffer);
}

size_t TransferBuffer::size() const {
    return bufferSize;
}

void* TransferBuffer::getInternal() {
    return buffer;
}

