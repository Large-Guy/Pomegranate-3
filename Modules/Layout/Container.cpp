#include "Container.h"

bool Container::Output::contains(float x, float y) const {
    return this->x <= x && x <= this->x + width && this->y <= y && y <= this->y + height;
}

Container::Scale::Scale() {
    this->label = Label::Auto;
    this->value = 0.0f;
}

Container::Scale::Scale(Label label, float value) {
    this->label = label;
    this->value = value;
}

Container::Scale& Container::Scale::operator=(float value) {
    this->value = value;
    return *this;
}

Container::Scale& Container::Scale::operator=(Label label) {
    this->label = label;
    return *this;
}

float Container::Scale::real(float relative, float autoSize, float scale) const {
    switch (label) {
        case Label::Auto:
            return autoSize;
        case Label::Percent:
            return value * relative;
        case Label::Physical:
            return value;
        case Label::Pixel:
            return value * scale;
        default:
            return value;
    }
}

float Container::Position::real(float start, float relative, float scale) const {
    switch (label) {
        case Label::Auto:
            return start;
        case Label::Percent:
            return start + value * relative;
        case Label::Pixel:
            return start + value * scale;
        case Label::Physical:
            return start + value;
        default:
            return start;
    }
}

Container::Position::Position() {
    this->label = Label::Auto;
    this->value = 0.0f;
}

Container::Position::Position(Label label, float value) {
    this->label = label;
    this->value = value;
}

Container::Position& Container::Position::operator=(float value) {
    this->value = value;
    return *this;
}

Container::Position& Container::Position::operator=(Label label) {
    this->label = label;
    return *this;
}

void Container::appendChild(const std::shared_ptr<Container>& container) {
    if (container->parent != nullptr) {
        container->parent->removeChild(getParentIndex());
    }
    this->children.push_back(container);
    container->parent = shared_from_this();
}

void Container::insertChild(size_t index, const std::shared_ptr<Container>& container) {
    this->children.insert(this->children.begin() + index, container);
    container->parent = shared_from_this();
}

void Container::removeChild(size_t index) {
    this->children[index]->parent = nullptr;
    this->children.erase(this->children.begin() + index);
}

size_t Container::getParentIndex() const {
    if (this->parent == nullptr)
        return 0;
    auto iter = std::find(this->parent->children.begin(), this->parent->children.end(), shared_from_this());
    return iter - this->parent->children.begin();
}

size_t Container::getChildCount() const {
    return children.size();
}

std::shared_ptr<Container> Container::getChild(size_t index) const {
    return children[index];
}

void Container::compute() {
    for (auto& child: children) {
        child->rect.x = child->x.real(rect.x, rect.width);
        child->rect.y = child->y.real(rect.y, rect.height);
        child->rect.width = child->width.real(rect.width, rect.width);
        child->rect.height = child->height.real(rect.height, rect.width);
    }
}

void Container::computeChildren() {
    if (this->parent == nullptr) {
        rect = {x.value, y.value, width.value, height.value};
    }

    compute();

    for (auto& child: children) {
        child->rect.x += this->xOffset.real(0.0f, rect.width);
        child->rect.y += this->yOffset.real(0.0f, rect.height);
        child->computeChildren();
    }
}

Container::Output Container::real() const {
    return rect;
}

Container::iterator Container::begin() {
    return children.begin();
}

Container::iterator Container::end() {
    return children.end();
}

Container::const_iterator Container::begin() const {
    return children.begin();
}

Container::const_iterator Container::end() const {
    return children.end();
}

Container::const_iterator Container::cbegin() const {
    return children.cbegin();
}

Container::const_iterator Container::cend() const {
    return children.cend();
}
