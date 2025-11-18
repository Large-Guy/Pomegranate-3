#include "UIElement.h"

UIElement::UIElement(const std::shared_ptr<Container>& container) {
    this->container = container;
    this->children = {};
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

void UIElement::addChild(const std::shared_ptr<UIElement>& child) {
    this->children.push_back(child);
    child->parent = shared_from_this();
    onChildAdded(child);
}

void UIElement::removeChild(int index) {
    this->children.erase(this->children.begin() + index);
    this->onChildRemoved(index);
}

std::shared_ptr<UIElement> UIElement::getChild(int index) {
    return children[index];
}

std::weak_ptr<UIElement> UIElement::getParent() const {
    return this->parent;
}

std::weak_ptr<UIElement> UIElement::getRoot() {
    auto current = weak_from_this();
    while (!current.lock()->getParent().expired()) {
        current = getParent();
    }
    return current;
}

UIElement::iterator UIElement::begin() {
    return children.begin();
}

UIElement::iterator UIElement::end() {
    return children.end();
}

UIElement::const_iterator UIElement::begin() const {
    return children.begin();
}

UIElement::const_iterator UIElement::end() const {
    return children.end();
}

UIElement::const_iterator UIElement::cbegin() const {
    return children.cbegin();
}

UIElement::const_iterator UIElement::cend() const {
    return children.cend();
}

void UIElement::onChildAdded(const std::shared_ptr<UIElement>& child) {
    container->appendChild(child->getContainer());
}

void UIElement::onChildRemoved(const int index) {
    container->removeChild(index);
}
