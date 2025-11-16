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

void UIElement::render(Viewport screen, const std::weak_ptr<CommandBuffer>& commandBuffer,
                       const std::shared_ptr<DrawPass>& drawPass,
                       const std::shared_ptr<Texture>& background) {
}

std::shared_ptr<Container> UIElement::getContainer() {
    return container;
}

void UIElement::addChild(const std::shared_ptr<UIElement>& child) {
    this->children.push_back(child);
    child->parent = shared_from_this();
    addChildImpl(child);
}

void UIElement::removeChild(int index) {
    this->children.erase(this->children.begin() + index);
    this->removeChildImpl(index);
}

std::shared_ptr<UIElement> UIElement::getChild(int index) {
    return children[index];
}

std::weak_ptr<UIElement> UIElement::getParent() {
    return this->parent;
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

void UIElement::addChildImpl(const std::shared_ptr<UIElement>& child) {
    container->appendChild(child->getContainer());
}

void UIElement::removeChildImpl(const int index) {
    container->removeChild(index);
}
