#include "Display.h"

#include "CopyPass.h"
#include "SDL3/SDL.h"

Display::Display(const std::shared_ptr<Renderer>& renderer, const std::shared_ptr<Window>& window) {
    this->window = window;
    this->renderer = renderer;

    if (SDL_GetGPUDeviceDriver(static_cast<SDL_GPUDevice*>(renderer->getInternal())) == std::string("d3d12") && window->
        hasFlag(Window::Flag::Transparent)) {
        this->sdlRenderer = SDL_CreateRenderer(static_cast<SDL_Window*>(window->getInternal()), nullptr);
        this->texture = nullptr;
        this->width = 0;
        this->height = 0;
        this->transferBuffer = nullptr;
    } else {
        auto device = static_cast<SDL_GPUDevice*>(renderer->getInternal());
        auto sdlWindow = static_cast<SDL_Window*>(window->getInternal());

        constexpr std::ptrdiff_t claimWindowOffset = 520;
        constexpr std::ptrdiff_t driverDataOffset = 664;

        using ClaimWindowFn = bool(*)(struct SDL_GPURenderer*, SDL_Window*);

        //due to a limitation of SDL GPU it doesn't allow transparent
        //windows, even though it does work on most platforms. We have to bypass this for performance. The one-
        //exception platform is accounted for differently. We will bypass the checks by simply getting the claim
        //function and calling it directly

        // copy function pointer from unaligned address
        ClaimWindowFn claimWindow = nullptr;
        std::memcpy(&claimWindow, reinterpret_cast<char*>(device) + claimWindowOffset, sizeof(claimWindow));

        // copy driverData pointer from device
        SDL_GPURenderer* driverData = nullptr;
        std::memcpy(&driverData, reinterpret_cast<char*>(device) + driverDataOffset, sizeof(driverData));
        if (!claimWindow || !driverData) {
            throw std::runtime_error("Failed to retrieve ClaimWindow or driverData");
        }
        if (!claimWindow(driverData,
                         sdlWindow)) {
            throw std::runtime_error("Failed to claim window for renderer");
        }
    }
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
    toDisplay = renderTexture;
    if (sdlRenderer == nullptr)
        return;
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
        this->transferBuffer = TransferBuffer::make(renderer, TransferBuffer::Type::Download,
                                                    width * height * bytes);
    }

    auto commandQueue = queue.lock();
    auto copyPass = CopyPass::make(renderer);
    copyPass->begin(commandQueue);

    renderTexture->download(copyPass, transferBuffer);

    copyPass->end();
}

void Display::present() {
    if (sdlRenderer != nullptr) {
        const void* textureInfo = transferBuffer->mapPtr();

        SDL_UpdateTexture(texture, nullptr, textureInfo,
                          width * SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32)->bytes_per_pixel);

        transferBuffer->unmap();

        SDL_RenderClear(this->sdlRenderer);
        SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_NONE);
        SDL_RenderTexture(sdlRenderer, texture, nullptr, nullptr);
        SDL_RenderPresent(sdlRenderer);
    } else {
        auto buffer = renderer->begin();
        auto cmd = buffer.lock();
        SDL_GPUTexture* swapchain;
        uint32_t width, height;
        if (!SDL_WaitAndAcquireGPUSwapchainTexture(static_cast<SDL_GPUCommandBuffer*>(cmd->getInternal()),
                                                   static_cast<SDL_Window*>(window->getInternal()), &swapchain,
                                                   &width, &height)) {
            fprintf(stderr, "ERROR: SDL_WaitAndAcquireGPUSwapchainTexture failed: %s\n", SDL_GetError());
            return;
        }

        SDL_GPUColorTargetInfo colorTargetInfo = {};
        colorTargetInfo.texture = swapchain;
        colorTargetInfo.clear_color = {0.0f, 0.0f, 0.0f, 1.0f};
        colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
        colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

        SDL_GPUBlitInfo blitInfo = {};
        blitInfo.source.texture = static_cast<SDL_GPUTexture*>(toDisplay->getInternal());
        blitInfo.source.x = 0;
        blitInfo.source.y = 0;
        blitInfo.source.w = width;
        blitInfo.source.h = height;

        blitInfo.destination.texture = swapchain;
        blitInfo.destination.x = 0;
        blitInfo.destination.y = 0;
        blitInfo.destination.w = width;
        blitInfo.destination.h = height;
        SDL_BlitGPUTexture(static_cast<SDL_GPUCommandBuffer*>(cmd->getInternal()), &blitInfo);

        renderer->end();
    }
    toDisplay = nullptr;
}

std::shared_ptr<Display> Display::make(const std::shared_ptr<Renderer>& renderer,
                                       const std::shared_ptr<Window>& window) {
    return std::shared_ptr<Display>(new Display(renderer, window));
}
