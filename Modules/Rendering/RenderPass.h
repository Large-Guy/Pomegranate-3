#ifndef POMEGRANATE_RENDERPASS_H
#define POMEGRANATE_RENDERPASS_H
#include "Color.h"
#include "Renderer.h"
#include "Texture.h"
#include "Viewport.h"


class RenderPass {
public:
    explicit RenderPass(const std::shared_ptr<Renderer>& renderer);
    virtual ~RenderPass() = default;

    virtual void begin(const std::weak_ptr<CommandQueue>& commandQueue) = 0;
    virtual void end() = 0;

    void* getInternal() const;
protected:
    std::shared_ptr<Renderer> renderer;
    void* renderPass;
};



#endif //POMEGRANATE_RENDERPASS_H