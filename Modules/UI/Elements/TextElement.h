#ifndef POMEGRANATE_TEXT_H
#define POMEGRANATE_TEXT_H
#include "ImageElement.h"
#include "Rendering/Font.h"
#include "UI/UIElement.h"


class TextElement : public UIElement {
public:
    std::string text;
    std::shared_ptr<Font> font;
    float fontSize = 16.0f;

    void onAddedToLayer(const std::shared_ptr<Renderer>& renderer) override;

    void update(float deltaTime) override;

    void render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                const std::weak_ptr<CommandBuffer>& commandBuffer, const std::shared_ptr<DrawPass>& drawPass,
                const std::shared_ptr<Texture>& background) override;

    static std::shared_ptr<TextElement> make();

protected:
    TextElement();

private:
    std::shared_ptr<Texture> texture;
    std::string previousText;

    std::shared_ptr<ImageElement> image;

    static struct TTF_TextEngine* textEngine;
};


#endif //POMEGRANATE_TEXT_H
