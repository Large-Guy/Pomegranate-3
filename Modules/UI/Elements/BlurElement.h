#ifndef POMEGRANATE_BLURPANEL_H
#define POMEGRANATE_BLURPANEL_H
#include "PanelElement.h"


class BlurElement : public PanelElement {
public:
    void onAddedToLayer(const std::shared_ptr<Renderer>& renderer) override;

    void render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                const std::weak_ptr<CommandBuffer>& commandBuffer, const std::shared_ptr<DrawPass>& drawPass,
                const std::shared_ptr<Texture>& background) override;

private:
    static std::shared_ptr<Pipeline> glassPipeline;
    static std::shared_ptr<Sampler> glassSampler;
};


#endif //POMEGRANATE_BLURPANEL_H
