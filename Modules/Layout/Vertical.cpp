#include "Vertical.h"

#include <iostream>
#include <ostream>

void Vertical::computeRect(float scale) {
    float xPadding = padding.real(rect.width, 0, scale, 1.0f);
    float yPadding = padding.real(rect.height, 0, scale, 1.0f);

    float paddedHeight = rect.height - yPadding * 2.0f;

    float totalPercent = 0.0f;
    float remainingPhysical = paddedHeight - gap.real(paddedHeight, 0, scale, 1.0f) * static_cast<float>(
                                  getChildCount() - 1);

    int autoCount = 0;

    for (auto& child: *this) {
        switch (child->height.label) {
            case Scale::Label::Auto:
                autoCount++;
                break;
            case Scale::Label::Percent:
                totalPercent += child->height.value;
                break;
            case Scale::Label::Pixel:
                remainingPhysical -= child->height.value * scale;
                break;
            case Scale::Label::Physical:
                remainingPhysical -= child->height.value;
                break;
        }
    }

    float percentScale = 1.0f;
    if ((yFill || (!yOverflow && totalPercent > 1.0f)) && totalPercent > 0.0f) {
        percentScale = 1.0f / totalPercent;
    }

    remainingPhysical -= paddedHeight * (totalPercent * percentScale);

    float alignmentOffset;

    switch (yAlignment) {
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

    for (auto& child: *this) {
        child->rect.x = rect.x + xPadding;
        child->rect.width = rect.width - xPadding * 2.0f;

        child->rect.y = rect.y + yPadding + consumed + alignmentOffset;
        child->rect.height = child->height.real(paddedHeight, autoSize, scale, percentScale);

        consumed += child->rect.height;

        if (child != *end()) {
            consumed += gap.real(paddedHeight, 0.0f, scale, percentScale);
        }
    }
}

