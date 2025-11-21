#include "Horizontal.h"

void Horizontal::computeRect(float scale) {
    float xPadding = padding.real(rect.width, 0, scale, 1.0f);
    float yPadding = padding.real(rect.height, 0, scale, 1.0f);

    float paddedWidth = rect.width - xPadding * 2.0f;

    float totalPercent = 0.0f;
    float remainingPhysical = paddedWidth - gap.real(paddedWidth, 0, scale, 1.0f) * static_cast<float>(
                                  getChildCount() - 1);
    int autoCount = 0;

    for (auto& child: *this) {
        switch (child->width.label) {
            case Scale::Label::Auto:
                autoCount++;
                break;
            case Scale::Label::Percent:
                totalPercent += child->width.value;
                break;
            case Scale::Label::Pixel:
                remainingPhysical -= child->width.value * scale;
                break;
            case Scale::Label::Physical:
                remainingPhysical -= child->width.value;
                break;
        }
    }

    float percentScale = 1.0f;
    if ((xFill || (!xOverflow && totalPercent > 1.0f)) && totalPercent > 0.0f) {
        percentScale = 1.0f / totalPercent;
    }

    remainingPhysical -= paddedWidth * (totalPercent * percentScale);

    float autoSize = remainingPhysical / static_cast<float>(autoCount);

    float alignmentOffset;

    switch (xAlignment) {
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

    for (auto& child: *this) {
        child->rect.y = rect.y + yPadding;
        child->rect.height = rect.height - yPadding * 2.0f;

        child->rect.x = rect.x + xPadding + consumed + alignmentOffset;
        child->rect.width = child->width.real(paddedWidth, autoSize, scale, percentScale);

        consumed += child->rect.width;

        if (child != *end()) {
            consumed += gap.real(paddedWidth, 0.0f, scale, percentScale);
        }
    }
}

std::shared_ptr<Horizontal> Horizontal::make() {
    return std::shared_ptr<Horizontal>(new Horizontal());
}
