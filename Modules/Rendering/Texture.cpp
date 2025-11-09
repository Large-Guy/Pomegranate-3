#include "Texture.h"

#include "CopyPass.h"
#include "RenderPass.h"

#include "SDL3/SDL_gpu.h"

Texture::Texture(const std::shared_ptr<Renderer>& renderer, Format format, int width, int height, BitFlag<Flags> flags) {
    SDL_GPUTextureCreateInfo info{};
    info.type = SDL_GPU_TEXTURETYPE_2D;
    info.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    SDL_GPUTextureUsageFlags usageFlags = SDL_GPU_TEXTUREUSAGE_SAMPLER;
    if (flags[RenderTarget])
        usageFlags |= SDL_GPU_TEXTUREUSAGE_COLOR_TARGET;
    info.usage = usageFlags;
    info.width = width;
    info.height = height;
    info.layer_count_or_depth = 1;
    info.num_levels = 1;
    info.sample_count = SDL_GPU_SAMPLECOUNT_1;

    this->renderer = renderer;
    this->textureWidth = width;
    this->textureHeight = height;
    this->format = format;

    texture = SDL_CreateGPUTexture(static_cast<SDL_GPUDevice*>(renderer->getInternal()), &info);
}

Texture::~Texture() {
    SDL_ReleaseGPUTexture(static_cast<SDL_GPUDevice*>(renderer->getInternal()), texture);
}

void Texture::download(const std::shared_ptr<CopyPass>& pass, const std::shared_ptr<TransferBuffer>& output) {
    SDL_GPUTextureRegion source{};
    source.texture = texture;
    source.w = textureWidth;
    source.h = textureHeight;
    source.d = 1;

    SDL_GPUTextureTransferInfo destination{};
    destination.transfer_buffer = static_cast<SDL_GPUTransferBuffer*>(output->getInternal());
    destination.offset = 0;

    SDL_DownloadFromGPUTexture(static_cast<SDL_GPUCopyPass*>(pass->getInternal()), &source, &destination);
}

int Texture::width() const {
    return textureWidth;
}

int Texture::height() const {
    return textureHeight;
}

void* Texture::getInternal() const {
    return texture;
}
