#include "Texture.h"

#include "CopyPass.h"
#include "RenderPass.h"

#include "SDL3/SDL_gpu.h"
#include "SDL3_image/SDL_image.h"

Texture::Texture(const std::shared_ptr<Renderer>& renderer, Format format, int width, int height, int layers,
                 BitFlag<Flags> flags) {
    SDL_GPUTextureCreateInfo info{};
    info.type = SDL_GPU_TEXTURETYPE_2D;
    if (layers > 1)
        info.type = SDL_GPU_TEXTURETYPE_2D_ARRAY;
    info.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    SDL_GPUTextureUsageFlags usageFlags = SDL_GPU_TEXTUREUSAGE_SAMPLER;
    if (flags[RenderTarget])
        usageFlags |= SDL_GPU_TEXTUREUSAGE_COLOR_TARGET;
    info.usage = usageFlags;
    info.width = width;
    info.height = height;
    info.layer_count_or_depth = layers;
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

void Texture::upload(const std::shared_ptr<class CopyPass>& renderPass, const std::shared_ptr<TransferBuffer>& input) {
    SDL_GPUTextureTransferInfo source{};
    source.transfer_buffer = static_cast<SDL_GPUTransferBuffer*>(input->getInternal());

    SDL_GPUTextureRegion destination{};
    destination.texture = texture;
    destination.w = textureWidth;
    destination.h = textureHeight;
    destination.d = 1;
    SDL_UploadToGPUTexture(static_cast<SDL_GPUCopyPass*>(renderPass->getInternal()), &source, &destination, false);
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

std::shared_ptr<Texture> Texture::make(const std::shared_ptr<Renderer>& renderer, Format format, int width, int height,
                                       int layers, BitFlag<Flags> flags) {
    return std::shared_ptr<Texture>(new Texture(renderer, format, width, height, layers, flags));
}

std::shared_ptr<Texture> Texture::load(const std::shared_ptr<Renderer>& renderer, const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    auto texture = make(renderer, Format::R8G8B8A8_SRGB, surface->w, surface->h, 1,
                        BitFlag<Flags>());
    size_t size = surface->h * surface->pitch;
    auto transferBuffer = TransferBuffer::make(renderer, TransferBuffer::Type::Upload,
                                               size);

    auto* mapped = transferBuffer->map<uint8_t>();
    std::memcpy(mapped, surface->pixels, size);
    transferBuffer->unmap();

    auto cmdQueue = renderer->begin();
    auto copy = CopyPass::make(renderer);
    copy->begin(cmdQueue);

    texture->upload(copy, transferBuffer);

    copy->end();

    renderer->end();

    SDL_DestroySurface(surface);
    return texture;
}
