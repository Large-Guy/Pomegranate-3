#include "Container.h"

bool Container::Output::contains(float x, float y) {
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

Container::Container(Type type, Position x, Position y, Scale width, Scale height) {
    this->type = type;

    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->padding = {Scale::Label::Auto, 0};
    this->gap = {Scale::Label::Auto, 0};
    this->overflowX = false;
    this->overflowY = false;
    this->fillX = false;
    this->fillY = false;
    this->alignment = Alignment::Leading;
    this->rect = {};
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

void Container::computeChildren() {
    if (this->parent == nullptr) {
        rect = {x.value, y.value, width.value, height.value};
    }

    switch (type) {
        case Type::Float:
            computeFloat();
            break;
        case Type::Full:
            computeFull();
            break;
        case Type::Vertical:
            computeVertical();
            break;
        case Type::Horizontal:
            computeHorizontal();
            break;
    }

    for (auto& child: children) {
        child->computeChildren();
    }
}

Container::Output Container::real() const {
    return rect;
}

void Container::computeFloat() {
    for (auto& child: children) {
        child->rect.x = child->x.real(rect.x, rect.width);
        child->rect.y = child->y.real(rect.y, rect.height);
        child->rect.width = child->width.real(rect.width, rect.width);
        child->rect.height = child->height.real(rect.height, rect.width);
    }
}

void Container::computeFull() {
    for (auto& child: children) {
        float xPadding = padding.real(rect.width, 0);
        float yPadding = padding.real(rect.height, 0);
        child->rect.x = rect.x + xPadding;
        child->rect.y = rect.y + yPadding;
        child->rect.width = rect.width - xPadding * 2.0f;
        child->rect.height = rect.height - yPadding * 2.0f;
    }
}

void Container::computeVertical() {
    float xPadding = padding.real(rect.width, 0);
    float yPadding = padding.real(rect.height, 0);

    float paddedHeight = rect.height - yPadding;

    float totalPercent = 0.0f;
    float remainingPhysical = paddedHeight - gap.real(paddedHeight, 0) * children.size();
    int autoCount = 0;

    for (auto& child: children) {
        switch (child->height.label) {
            case Scale::Label::Auto:
                autoCount++;
                break;
            case Scale::Label::Percent:
                totalPercent += child->height.value;
                break;
            case Scale::Label::Pixel:
                remainingPhysical -= child->height.value; //TODO: Implement scaling
                break;
            case Scale::Label::Physical:
                remainingPhysical -= child->height.value;
                break;
        }
    }

    float percentScale = 1.0f;
    if ((fillY || (!overflowY && totalPercent > 1.0f)) && totalPercent > 0.0f) {
        percentScale = 1.0f / totalPercent;
    }

    remainingPhysical -= paddedHeight * (totalPercent * percentScale);

    float alignmentOffset;

    switch (alignment) {
        case Alignment::Leading:
            alignmentOffset = 0.0f;
            break;
        case Alignment::Center:
            alignmentOffset = remainingPhysical / 2.0f;
            break;
        case Alignment::Trailing:
            alignmentOffset = remainingPhysical;
            break;
        default:
            throw std::runtime_error("Invalid alignment");
    }

    float autoSize = remainingPhysical / static_cast<float>(autoCount);

    float consumed = 0.0f;

    for (auto& child: children) {
        child->rect.x = rect.x + xPadding;
        child->rect.width = rect.width - xPadding * 2.0f;

        child->rect.y = rect.y + yPadding + consumed + alignmentOffset;
        child->rect.height = child->height.real(paddedHeight, autoSize);

        consumed += child->rect.height;

        if (child != *children.end()) {
            consumed += gap.real(paddedHeight, 0.0f);
        }
    }
}

void Container::computeHorizontal() {
    float xPadding = padding.real(rect.width, 0);
    float yPadding = padding.real(rect.height, 0);

    float paddedWidth = rect.width - xPadding;

    float totalPercent = 0.0f;
    float remainingPhysical = paddedWidth - gap.real(paddedWidth, 0) * children.size();
    int autoCount = 0;

    for (auto& child: children) {
        switch (child->width.label) {
            case Scale::Label::Auto:
                autoCount++;
                break;
            case Scale::Label::Percent:
                totalPercent += child->width.value;
                break;
            case Scale::Label::Pixel:
                remainingPhysical -= child->width.value; //TODO: Implement scaling
                break;
            case Scale::Label::Physical:
                remainingPhysical -= child->width.value;
                break;
        }
    }

    float percentScale = 1.0f;
    if ((fillX || (!overflowX && totalPercent > 1.0f)) && totalPercent > 0.0f) {
        percentScale = 1.0f / totalPercent;
    }

    remainingPhysical -= paddedWidth * (totalPercent * percentScale);

    float autoSize = remainingPhysical / static_cast<float>(autoCount);

    float alignmentOffset;

    switch (alignment) {
        case Alignment::Leading:
            alignmentOffset = 0.0f;
            break;
        case Alignment::Center:
            alignmentOffset = remainingPhysical / 2.0f;
            break;
        case Alignment::Trailing:
            alignmentOffset = remainingPhysical;
            break;
        default:
            throw std::runtime_error("Invalid alignment");
    }

    float consumed = 0.0f;

    for (auto& child: children) {
        child->rect.y = rect.y + yPadding;
        child->rect.height = rect.height - yPadding * 2.0f;

        child->rect.x = rect.x + xPadding + consumed + alignmentOffset;
        child->rect.width = child->width.real(paddedWidth, autoSize);

        consumed += child->rect.width;

        if (child != *children.end()) {
            consumed += gap.real(paddedWidth, 0.0f);
        }
    }
}
