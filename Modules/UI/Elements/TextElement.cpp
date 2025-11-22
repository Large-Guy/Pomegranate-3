#include "TextElement.h"

#include "ImageElement.h"
#include "SDL3_ttf/SDL_ttf.h"

TTF_TextEngine* TextElement::textEngine = nullptr;

TextElement::TextElement() : UIElement(Container::make()) {
}

void TextElement::onAddedToLayer(const std::shared_ptr<Renderer>& renderer) {
    if (textEngine == nullptr) {
        textEngine = TTF_CreateGPUTextEngine(static_cast<SDL_GPUDevice*>(renderer->getInternal()));
    }
}

void TextElement::update(float deltaTime) {
    if (font == nullptr)
        return;
    if (previousText != text) {
        //Regenerate
        //texture = font->render(text, fontSize);
        previousText = text;
    }
}

void TextElement::render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                         const std::weak_ptr<CommandBuffer>& commandBuffer, const std::shared_ptr<DrawPass>& drawPass,
                         const std::shared_ptr<Texture>& background) {
    image->render(screen, scale, theme, commandBuffer, drawPass, background);
}

std::shared_ptr<TextElement> TextElement::make() {
    auto element = std::shared_ptr<TextElement>(new TextElement());
    //Init stuff
    auto textDisplay = ImageElement::make();
    element->image = textDisplay;
    element->addPrivateChild(textDisplay);
    return element;
}

