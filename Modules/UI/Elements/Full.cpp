#include "Full.h"

#include "Layout/Container.h"
#include "Layout/Fill.h"

Full::Full() : UIElement(std::make_shared<Fill>()) {
    getContainer()->padding = {Container::Scale::Label::Pixel, 8.0f};
}
