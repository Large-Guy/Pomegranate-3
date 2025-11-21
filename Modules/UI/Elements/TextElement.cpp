#include "TextElement.h"

#include "ImageElement.h"

TextElement::TextElement() : UIElement(Container::make()) {
}

void TextElement::onAddedToLayer(const std::shared_ptr<Renderer>& renderer) {
}

void TextElement::render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                         const std::weak_ptr<CommandBuffer>& commandBuffer, const std::shared_ptr<DrawPass>& drawPass,
                         const std::shared_ptr<Texture>& background) {
}

std::shared_ptr<TextElement> TextElement::make() {
    return std::shared_ptr<TextElement>(new TextElement());
}

