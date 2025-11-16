#ifndef POMEGRANATE_PANEL_H
#define POMEGRANATE_PANEL_H
#include "Rendering/Pipeline.h"
#include "UI/UIElement.h"


class Panel : public UIElement {
public:
    Panel();

    void onAddedToLayer(const std::shared_ptr<Renderer>& renderer) override;

    void render(Viewport screen, const std::weak_ptr<CommandBuffer>& commandBuffer,
                const std::shared_ptr<DrawPass>& drawPass,
                const std::shared_ptr<Texture>& background) override;

private:
    struct alignas(16) PanelUniform {
        float screenX, screenY, screenW, screenH;
        float rectX, rectY, rectW, rectH;
    };

    std::shared_ptr<Pipeline> pipeline;
};


#endif //POMEGRANATE_PANEL_H
