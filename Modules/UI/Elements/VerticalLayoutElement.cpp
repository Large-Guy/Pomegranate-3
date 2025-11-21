#include "VerticalLayoutElement.h"

#include "Layout/Container.h"
#include "Layout/Vertical.h"

std::shared_ptr<VerticalLayoutElement> VerticalLayoutElement::make() {
    return std::shared_ptr<VerticalLayoutElement>(new VerticalLayoutElement());
}

VerticalLayoutElement::VerticalLayoutElement() : UIElement(Vertical::make()) {
    auto container = std::dynamic_pointer_cast<Vertical>(getContainer());
    container->gap = {Container::Scale::Label::Pixel, 8.0f};
}
