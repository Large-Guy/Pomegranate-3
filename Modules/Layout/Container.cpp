#include "Container.h"

#include <iostream>

bool Container::Output::contains(float x, float y, bool scale) const {
    if (scale) {
        x *= this->scale;
        y *= this->scale;
    }
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

float Container::Scale::real(float relative, float autoSize, float scale, float percentScale) const {
    switch (label) {
        case Label::Auto:
            return autoSize;
        case Label::Percent:
            return value * relative * percentScale;
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

Container::Container() {
    reg("x", x);
    reg("y", y);
    reg("width", width);
    reg("height", height);
    reg("padding", padding);
    reg("gap", gap);
    reg("xPivot", xPivot);
    reg("yPivot", yPivot);
}

void Container::computeRect(float scale) {
    for (const auto& child: *this) {
        child->rect.x = child->x.real(rect.x, rect.width, scale);
        child->rect.y = child->y.real(rect.y, rect.height, scale);
        child->rect.width = child->width.real(rect.width, rect.width, scale, 1.0f);
        child->rect.height = child->height.real(rect.height, rect.height, scale, 1.0f);
    }
}

void Container::compute(float scale) {
    if (getParent() == nullptr) {
        rect = {x.value, y.value, width.value, height.value, scale};
    }

    this->rect.x -= this->xPivot.real(0, rect.width, scale);
    this->rect.y -= this->yPivot.real(0, rect.height, scale);

    computeRect(scale);

    for (auto& child: *this) {
        child->compute(scale);
    }

    rect.scale = scale;
}

Container::Output Container::real() const {
    return rect;
}
