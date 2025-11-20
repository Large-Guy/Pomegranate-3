#ifndef POMEGRANATE_TEXT_H
#define POMEGRANATE_TEXT_H
#include "UI/UIElement.h"


class TextElement : public UIElement {
public:
    std::string text;

    TextElement();

    void onAddedToLayer(const std::shared_ptr<Renderer>& renderer) override;

    void render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                const std::weak_ptr<CommandBuffer>& commandBuffer, const std::shared_ptr<DrawPass>& drawPass,
                const std::shared_ptr<Texture>& background) override;
};


#endif //POMEGRANATE_TEXT_H
