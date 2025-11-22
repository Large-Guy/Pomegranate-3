#ifndef POMEGRANATE_UICOMPOSITOR_H
#define POMEGRANATE_UICOMPOSITOR_H
#include <memory>
#include <vector>

#include "UILayer.h"
#include "Rendering/CommandBuffer.h"
#include "Rendering/Pipeline.h"
#include "Rendering/Texture.h"


class UICompositor {
public:
    void addLayer(const std::shared_ptr<UILayer>& layer);

    void update(float deltaTime);

    std::shared_ptr<Texture> render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                                    const std::weak_ptr<CommandBuffer>& commandBuffer);

    void pushEvent(Event& event) const;

    static std::shared_ptr<UICompositor> make(const std::shared_ptr<Renderer>& renderer);

private:
    explicit UICompositor(const std::shared_ptr<Renderer>& renderer);

    std::shared_ptr<Renderer> renderer;

    std::shared_ptr<Texture> pong;
    std::shared_ptr<Texture> ping;

    std::shared_ptr<Sampler> sampler;

    std::vector<std::shared_ptr<UILayer> > layers;

    std::shared_ptr<Pipeline> pipeline;

    friend class UILayer;
};


#endif //POMEGRANATE_UICOMPOSITOR_H
