#include "TextElement.h"

#include "ImageElement.h"

TextElement::TextElement() : UIElement(std::make_shared<Container>()) {
}

void TextElement::onAddedToLayer(const std::shared_ptr<Renderer>& renderer) {
}

void TextElement::render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                         const std::weak_ptr<CommandBuffer>& commandBuffer, const std::shared_ptr<DrawPass>& drawPass,
                         const std::shared_ptr<Texture>& background) {
}

