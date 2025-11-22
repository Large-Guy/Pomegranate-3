#include "UIElement.h"

#include "Events/Event.h"

std::weak_ptr<UIElement> focused = {};

UIElement::UIElement(const std::shared_ptr<Container>& container) {
    this->container = container;
}

UIElement::~UIElement() = default;

std::shared_ptr<Container> UIElement::getContainer() {
    return container;
}

void UIElement::standardEventHandler(Event& event) {
    if (const auto mouseMotion = event.as<MouseMoveEvent>()) {
        if (hovering) {
            if (!container->real().contains(mouseMotion->x, mouseMotion->y, true)) {
                onCurserExit();
                hovering = false;
            }
        } else {
            if (container->real().contains(mouseMotion->x, mouseMotion->y, true)) {
                onCursorEnter();
                hovering = true;
            }
        }
    } else if (const auto mousePress = event.as<MousePressEvent>()) {
        if (!hovering)
            return;
        onPressed(mousePress->button);
        pressed = true;
    } else if (const auto mouseRelease = event.as<MouseReleaseEvent>()) {
        if (!pressed)
            return;
        onReleased(mouseRelease->button);
        pressed = false;
    }
}

UIElement::EventResult UIElement::onEvent(Event& event) {
    return EventResult::Propagate;
}

void UIElement::onAddedToLayer(const std::shared_ptr<Renderer>& renderer) {
}

void UIElement::onRemovedFromLayer() {
}

void UIElement::onCursorEnter() {
}

void UIElement::onCurserExit() {
}

void UIElement::onPressed(MouseEvent::Button button) {
}

void UIElement::onReleased(MouseEvent::Button button) {
}

void UIElement::update(float deltaTime) {
}

void UIElement::render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                       const std::weak_ptr<CommandBuffer>& commandBuffer,
                       const std::shared_ptr<DrawPass>& drawPass,
                       const std::shared_ptr<Texture>& background) {
}

bool UIElement::isHovered() const {
    return hovering;
}

void UIElement::onChildAdded(const std::shared_ptr<UIElement>& child) {
    container->addChild(child->getContainer());
}

void UIElement::onChildRemoved(const std::shared_ptr<UIElement>& child) {
    container->removeChild(child->getContainer());
}
