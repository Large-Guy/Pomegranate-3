#include "DrawPass.h"

#include "SDL3/SDL_gpu.h"

DrawPass::DrawPass(const std::shared_ptr<Renderer>& renderer) : RenderPass(renderer) {
    this->renderer = renderer;

    this->renderPass = nullptr;
    this->clearColor = {};
    this->vp = {};
    this->outputTexture = nullptr;
    this->elements = {};
}

DrawPass::~DrawPass() {
    if (this->renderPass != nullptr)
        DrawPass::end();
}

void DrawPass::clear(Color color) {
    this->elements.set(ElementClear, true);
    this->clearColor = color;
}

void DrawPass::viewport(Viewport viewport) {
    this->elements.set(ElementViewport, true);
    this->vp = viewport;
}

void DrawPass::texture(std::shared_ptr<Texture> texture, int layer) {
    this->elements.set(ElementTexture, true);
    this->outputTexture = std::move(texture);
    this->layer = layer;
}

void DrawPass::disableClear() {
    this->elements.set(ElementClear, false);
}

void DrawPass::disableViewport() {
    this->elements.set(ElementViewport, false);
}

void DrawPass::disableTexture() {
    this->elements.set(ElementTexture, false);
}


void DrawPass::begin(const std::weak_ptr<CommandBuffer>& commandQueue) {
    if (commandQueue.expired()) {
        throw std::runtime_error("CommandQueue has expired");
    }

    std::shared_ptr<CommandBuffer> commandBuffer = commandQueue.lock();
    SDL_GPUColorTargetInfo colorTargetInfo = {};

    if (elements[ElementTexture]) {
        colorTargetInfo.texture = static_cast<SDL_GPUTexture*>(this->outputTexture->getInternal());
        colorTargetInfo.layer_or_depth_plane = layer;
    }
    if (elements[ElementClear]) {
        colorTargetInfo.clear_color = SDL_FColor{clearColor.r, clearColor.g, clearColor.b, clearColor.a};
        colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
    }
    colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

    this->renderPass = SDL_BeginGPURenderPass(static_cast<SDL_GPUCommandBuffer*>(commandBuffer->getInternal()),
                                              &colorTargetInfo, 1, nullptr);

    if (elements[ElementViewport]) {
        SDL_GPUViewport viewport = {};
        viewport.x = this->vp.x;
        viewport.y = this->vp.y + this->vp.h;
        viewport.w = this->vp.w;
        viewport.h = -this->vp.h;
        viewport.min_depth = this->vp.near;
        viewport.max_depth = this->vp.far;

        SDL_SetGPUViewport(static_cast<SDL_GPURenderPass*>(this->renderPass), &viewport);
    }
}

void DrawPass::end() {
    if (renderPass == nullptr)
        throw std::runtime_error("RenderPass has not been started");
    SDL_EndGPURenderPass(static_cast<SDL_GPURenderPass*>(renderPass));
    renderPass = nullptr;
}

void DrawPass::scissor(float x, float y, float width, float height) const {
    if (renderPass == nullptr)
        throw std::runtime_error("Draw Pass has not been started");
    SDL_Rect scissor = {static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height)};
    SDL_SetGPUScissor(static_cast<SDL_GPURenderPass*>(renderPass), &scissor);
}

void DrawPass::disableScissor() {
    if (renderPass == nullptr)
        throw std::runtime_error("Draw Pass has not been started");
    SDL_SetGPUScissor(static_cast<SDL_GPURenderPass*>(renderPass), nullptr);
}


void DrawPass::drawPrimitives(int verticesCount, int vertexStart, int instancesCount, int instanceStart) const {
    if (renderPass == nullptr)
        throw std::runtime_error("Draw Pass has not been started");
    SDL_DrawGPUPrimitives(static_cast<SDL_GPURenderPass*>(renderPass), verticesCount, instancesCount, vertexStart,
                          instanceStart);
}

std::shared_ptr<DrawPass> DrawPass::make(const std::shared_ptr<Renderer>& renderer) {
    return std::shared_ptr<DrawPass>(new DrawPass(renderer));
}
