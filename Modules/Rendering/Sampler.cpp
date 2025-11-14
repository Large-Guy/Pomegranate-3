#include "Sampler.h"

#include <stdexcept>

#include "SDL3/SDL_gpu.h"

Sampler::Sampler(const std::shared_ptr<Renderer>& renderer, Filter filter, Wrap wrap) {
    this->renderer = renderer;
    this->filter = filter;
    this->wrap = wrap;

    SDL_GPUSamplerCreateInfo info = {};
    SDL_GPUFilter samplerFilter;
    switch (filter) {
        case Filter::Nearest:
            samplerFilter = SDL_GPU_FILTER_NEAREST;
            break;
        case Filter::Linear:
            samplerFilter = SDL_GPU_FILTER_LINEAR;
            break;
        default:
            throw std::runtime_error("Invalid filter");
    }

    SDL_GPUSamplerAddressMode mode;
    switch (wrap) {
        case Wrap::Clamp:
            mode = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
            break;
        case Wrap::Repeat:
            mode = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
            break;
        case Wrap::Mirrored:
            mode = SDL_GPU_SAMPLERADDRESSMODE_MIRRORED_REPEAT;
            break;
        default:
            throw std::runtime_error("Invalid wrap");
    }

    info.mag_filter = samplerFilter;
    info.min_filter = samplerFilter;
    info.address_mode_u = mode;
    info.address_mode_v = mode;
    info.address_mode_w = mode;
    info.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;

    this->sampler = SDL_CreateGPUSampler(static_cast<SDL_GPUDevice*>(renderer->getInternal()), &info);
}

void* Sampler::getInternal() {
    return this->sampler;
}
