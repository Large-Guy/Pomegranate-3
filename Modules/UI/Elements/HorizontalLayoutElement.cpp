#include "HorizontalLayoutElement.h"

#include "Layout/Horizontal.h"


std::shared_ptr<HorizontalLayoutElement> HorizontalLayoutElement::make() {
    return std::shared_ptr<HorizontalLayoutElement>(new HorizontalLayoutElement());
}

HorizontalLayoutElement::HorizontalLayoutElement() : UIElement(Horizontal::make()) {
    auto container = std::dynamic_pointer_cast<Horizontal>(getContainer());
    container->gap = {Container::Scale::Label::Pixel, 8.0f};
}
