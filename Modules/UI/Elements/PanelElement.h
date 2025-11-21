#ifndef POMEGRANATE_PANEL_H
#define POMEGRANATE_PANEL_H
#include "Rendering/Pipeline.h"
#include "UI/UIElement.h"


class PanelElement : public UIElement {
public:
    void onAddedToLayer(const std::shared_ptr<Renderer>& renderer) override;

    void render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                const std::weak_ptr<CommandBuffer>& commandBuffer,
                const std::shared_ptr<DrawPass>& drawPass,
                const std::shared_ptr<Texture>& background) override;

    std::string fill = "primary-fill";
    std::string border = "primary-border";

    static std::shared_ptr<PanelElement> make();

protected:
    PanelElement();

    struct alignas(16) PanelUniform {
        float screenX, screenY, screenW, screenH;
        float rectX, rectY, rectW, rectH;

        Color fill;
        Color border;
    };

    std::shared_ptr<Pipeline> pipeline;

    static std::shared_ptr<Pipeline> standardPipeline;
};


#endif //POMEGRANATE_PANEL_H
