#include "Buffer.h"

#include <stdexcept>

#include "CopyPass.h"
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

void Buffer::upload(const std::shared_ptr<CopyPass>& pass, const std::shared_ptr<TransferBuffer>& data) {
    SDL_GPUTransferBufferLocation location{};
    location.offset = 0;
    location.transfer_buffer = static_cast<SDL_GPUTransferBuffer*>(data->getInternal());

    SDL_GPUBufferRegion region{};
    region.offset = 0;
    region.size = std::min(size, data->size());
    region.buffer = this->buffer;

    SDL_UploadToGPUBuffer(static_cast<SDL_GPUCopyPass*>(pass->getInternal()), &location, &region, false);
}

void Buffer::bind(const std::shared_ptr<DrawPass>& pass) const {
    const SDL_GPUBufferBinding binding{
        .buffer = buffer,
        .offset = 0,
    };
    SDL_BindGPUVertexBuffers(static_cast<SDL_GPURenderPass*>(pass->getInternal()), 0, &binding, 1);
}

Buffer::Type Buffer::type() {
    return usage;
}
