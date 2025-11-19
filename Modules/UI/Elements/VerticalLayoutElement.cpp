#include "VerticalLayoutElement.h"

#include "Layout/Container.h"
#include "Layout/Vertical.h"

VerticalLayoutElement::VerticalLayoutElement() : UIElement(std::make_shared<Vertical>()) {
    auto container = std::dynamic_pointer_cast<Vertical>(getContainer());
    container->gap = {Container::Scale::Label::Pixel, 8.0f};
}
