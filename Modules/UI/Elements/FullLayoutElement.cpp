#include "FullLayoutElement.h"

#include "Layout/Container.h"
#include "Layout/Fill.h"

FullLayoutElement::FullLayoutElement() : UIElement(std::make_shared<Fill>()) {
    getContainer()->padding = {Container::Scale::Label::Pixel, 8.0f};
}
