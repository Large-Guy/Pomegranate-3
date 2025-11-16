#include "Display.h"

#include "CopyPass.h"
#include "SDL3/SDL.h"

Display::Display(const std::shared_ptr<Renderer>& renderer, const std::shared_ptr<Window>& window) {
    this->window = window;
    this->renderer = renderer;
    this->sdlRenderer = SDL_CreateRenderer(static_cast<SDL_Window*>(window->getInternal()), nullptr);
    this->texture = nullptr;
    this->width = 0;
    this->height = 0;
    this->transferBuffer = nullptr;
}

Display::~Display() {
    if (this->texture) {
        SDL_DestroyTexture(this->texture);
    }
    if (this->sdlRenderer) {
        SDL_DestroyRenderer(this->sdlRenderer);
    }
}

void Display::target(const std::weak_ptr<CommandBuffer>& queue, const std::shared_ptr<Texture>& renderTexture) {
    if (queue.expired()) {
        return;
    }
    if (this->texture == nullptr || renderTexture->width() != this->width || renderTexture->height() != this->
        height) {
        if (this->texture != nullptr) {
            SDL_DestroyTexture(this->texture);
        }
        this->width = renderTexture->width();
        this->height = renderTexture->height();
        this->texture = SDL_CreateTexture(this->sdlRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width,
                                          height);
        int bytes = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32)->bytes_per_pixel;
        this->transferBuffer = std::make_shared<TransferBuffer>(renderer, TransferBuffer::Type::Download,
                                                                width * height * bytes);
    }

    auto commandQueue = queue.lock();
    auto copyPass = std::make_shared<CopyPass>(renderer);
    copyPass->begin(commandQueue);

    renderTexture->download(copyPass, transferBuffer);

    copyPass->end();
}

void Display::present() const {
    const void* textureInfo = transferBuffer->mapPtr();

    SDL_UpdateTexture(texture, nullptr, textureInfo,
                      width * SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32)->bytes_per_pixel);

    transferBuffer->unmap();

    SDL_RenderClear(this->sdlRenderer);
    SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_NONE);
    SDL_RenderTexture(sdlRenderer, texture, nullptr, nullptr);
    SDL_RenderPresent(sdlRenderer);
}
