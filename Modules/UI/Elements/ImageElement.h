#ifndef POMEGRANATE_IMAGEELEMENT_H
#define POMEGRANATE_IMAGEELEMENT_H
#include "Rendering/Pipeline.h"
#include "UI/UIElement.h"


class ImageElement : public UIElement {
public:
    std::shared_ptr<Texture> image;
    std::shared_ptr<Sampler> sampler;

    ImageElement();

    void onAddedToLayer(const std::shared_ptr<Renderer>& renderer) override;

    void render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                const std::weak_ptr<CommandBuffer>& commandBuffer, const std::shared_ptr<DrawPass>& drawPass,
                const std::shared_ptr<Texture>& background) override;

protected:
    struct alignas(16) Uniform {
        float screenX, screenY, screenW, screenH;
        float rectX, rectY, rectW, rectH;
    };

    std::shared_ptr<Pipeline> pipeline;

    static std::shared_ptr<Pipeline> standardPipeline;
};


#endif //POMEGRANATE_IMAGEELEMENT_H
