#include "SpacerElement.h"

std::shared_ptr<SpacerElement> SpacerElement::make() {
    return std::shared_ptr<SpacerElement>(new SpacerElement());
}

SpacerElement::SpacerElement() : UIElement(Container::make()) {
}
