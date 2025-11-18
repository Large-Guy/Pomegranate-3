#include "UIElement.h"

UIElement::UIElement(const std::shared_ptr<Container>& container) {
    this->container = container;
}

UIElement::~UIElement() = default;

void UIElement::onAddedToLayer(const std::shared_ptr<Renderer>& renderer) {
}

void UIElement::onRemovedFromLayer() {
}

void UIElement::render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                       const std::weak_ptr<CommandBuffer>& commandBuffer,
                       const std::shared_ptr<DrawPass>& drawPass,
                       const std::shared_ptr<Texture>& background) {
}

std::shared_ptr<Container> UIElement::getContainer() {
    return container;
}

void UIElement::onChildAdded(const std::shared_ptr<UIElement>& child) {
    container->addChild(child->getContainer());
}

void UIElement::onChildRemoved(const std::shared_ptr<UIElement>& child) {
    container->removeChild(child->getContainer());
}
