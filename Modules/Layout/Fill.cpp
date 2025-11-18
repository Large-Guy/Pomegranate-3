#include "Fill.h"

void Fill::computeRect(float scale) {
    for (auto& child: *this) {
        float xPadding = padding.real(rect.width, 0, scale, 1.0f);
        float yPadding = padding.real(rect.height, 0, scale, 1.0f);
        child->rect.x = rect.x + xPadding;
        child->rect.y = rect.y + yPadding;
        child->rect.width = rect.width - xPadding * 2.0f;
        child->rect.height = rect.height - yPadding * 2.0f;
    }
}
