#ifndef POMEGRANATE_RENDERPASS_H
#define POMEGRANATE_RENDERPASS_H
#include "Color.h"
#include "Renderer.h"
#include "Texture.h"
#include "Viewport.h"


class RenderPass {
public:
    virtual ~RenderPass() = default;

    virtual void begin(const std::weak_ptr<CommandBuffer>& commandQueue) = 0;

    virtual void end() = 0;

    [[nodiscard]] void* getInternal() const;

protected:
    explicit RenderPass(const std::shared_ptr<Renderer>& renderer);


    std::shared_ptr<Renderer> renderer;
    void* renderPass;
};


#endif //POMEGRANATE_RENDERPASS_H
