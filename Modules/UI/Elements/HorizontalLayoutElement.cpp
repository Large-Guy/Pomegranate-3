#include "HorizontalLayoutElement.h"

#include "Layout/Horizontal.h"


HorizontalLayoutElement::HorizontalLayoutElement() : UIElement(std::make_shared<Horizontal>()) {
    auto container = std::dynamic_pointer_cast<Horizontal>(getContainer());
    container->gap = {Container::Scale::Label::Pixel, 8.0f};
}
