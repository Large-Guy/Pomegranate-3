#include "FullLayoutElement.h"

#include "Layout/Container.h"
#include "Layout/Fill.h"

std::shared_ptr<FullLayoutElement> FullLayoutElement::make() {
    return std::shared_ptr<FullLayoutElement>(new FullLayoutElement());
}

FullLayoutElement::FullLayoutElement() : UIElement(Fill::make()) {
    getContainer()->padding = {Container::Scale::Label::Pixel, 8.0f};
}
