#include "UILayer.h"

#include "UICompositor.h"

UILayer::UILayer(const std::shared_ptr<Renderer>& renderer) {
    this->texture = nullptr;
    this->renderer = renderer;
}

std::shared_ptr<Texture> UILayer::render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                                         const std::weak_ptr<CommandBuffer>& commandBuffer,
                                         const std::shared_ptr<Texture>& background) {
    if (this->texture == nullptr || static_cast<float>(this->texture->width()) != screen.w || static_cast<float>(this->
            texture->height()) != screen.h) {
        this->texture = std::make_shared<Texture>(renderer, Texture::Format::R8G8B8A8_SRGB, screen.w, screen.h, 1,
                                                  BitFlag<Texture::Flags>({Texture::Flags::RenderTarget}));
    }
    auto drawPass = std::make_shared<DrawPass>(renderer);
    drawPass->texture(texture);
    drawPass->clear({0.0f, 0.0f, 0.0f, 0.0f});
    drawPass->viewport(screen);
    drawPass->begin(commandBuffer);

    drawPass->scissor(screen.x, screen.y, screen.w, screen.h);

    for (const auto& element: elements) {
        drawElements(screen, scale, theme, commandBuffer, drawPass, background, element);
    }

    drawPass->end();

    return texture;
}

static bool propagateEvents(const std::shared_ptr<UIElement>& element, Event& event) {
    for (auto& child: *element) {
        if (propagateEvents(child, event))
            return true;
    }
    element->standardEventHandler(event);
    auto eventResult = element->onEvent(event);
    return eventResult == UIElement::EventResult::Consumed;
}

void UILayer::pushEvent(Event& event) {
    for (const auto& element: elements) {
        if (propagateEvents(element, event))
            return;
    }
}

static void recurseElementAdded(const std::shared_ptr<Renderer>& renderer, const std::shared_ptr<UIElement>& element) {
    element->onAddedToLayer(renderer);
    for (const auto& child: *element) {
        recurseElementAdded(renderer, std::dynamic_pointer_cast<UIElement>(child));
    }
}


static void recurseElementRemoved(const std::shared_ptr<Renderer>& renderer,
                                  const std::shared_ptr<UIElement>& element) {
    element->onRemovedFromLayer();
    for (const auto& child: *element) {
        recurseElementRemoved(renderer, std::dynamic_pointer_cast<UIElement>(child));
    }
}

void UILayer::addElement(const std::shared_ptr<UIElement>& element) {
    this->elements.push_back(element);
    recurseElementAdded(renderer, element);
    //TODO: We also need to account for when a child is added to an element later
}

void UILayer::removeElement(int index) {
    recurseElementRemoved(renderer, this->elements[index]);
    this->elements.erase(this->elements.begin() + index);
}

void UILayer::drawElements(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                           const std::weak_ptr<CommandBuffer>& commandBuffer,
                           const std::shared_ptr<DrawPass>& drawPass,
                           const std::shared_ptr<Texture>& background, const std::shared_ptr<UIElement>& element) {
    if (const auto parent = element->getParent<UIElement>()) {
        const auto container = parent->getContainer();
        auto rect = container->real();
        drawPass->scissor(rect.x, rect.y, rect.width, rect.height);
    }
    element->render(screen, scale, theme, commandBuffer, drawPass, background);
    for (const auto& child: *element) {
        drawElements(screen, scale, theme, commandBuffer, drawPass, background,
                     std::dynamic_pointer_cast<UIElement>(child));
    }
}
