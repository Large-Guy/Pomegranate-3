#include "UILayer.h"

UILayer::UILayer(const std::shared_ptr<Renderer>& renderer) {
    this->texture = nullptr;
    this->renderer = renderer;
}

std::shared_ptr<Texture> UILayer::render(Viewport screen, const std::weak_ptr<CommandBuffer>& commandBuffer,
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

    for (const auto& element: elements) {
        drawElements(screen, commandBuffer, drawPass, background, element);
    }

    drawPass->end();

    return texture;
}

static void recurseElementAdded(const std::shared_ptr<Renderer>& renderer, const std::shared_ptr<UIElement>& element) {
    element->onAddedToLayer(renderer);
    for (const auto& child: *element) {
        recurseElementAdded(renderer, child);
    }
}


static void recurseElementRemoved(const std::shared_ptr<Renderer>& renderer,
                                  const std::shared_ptr<UIElement>& element) {
    element->onRemovedFromLayer();
    for (const auto& child: *element) {
        recurseElementRemoved(renderer, child);
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

void UILayer::drawElements(Viewport screen, const std::weak_ptr<CommandBuffer>& commandBuffer,
                           const std::shared_ptr<DrawPass>& drawPass,
                           const std::shared_ptr<Texture>& background, const std::shared_ptr<UIElement>& element) {
    if (const auto parent = element->getParent().lock()) {
        const auto container = parent->getContainer();
        auto [x, y, width, height] = container->real();
        drawPass->scissor(x, y, width, height);
    }
    element->render(screen, commandBuffer, drawPass, background);
    for (const auto& child: *element) {
        drawElements(screen, commandBuffer, drawPass, background, child);
    }
}
