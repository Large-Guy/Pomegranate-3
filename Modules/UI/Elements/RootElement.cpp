#include "RootElement.h"

RootElement::RootElement() : UIElement(std::make_shared<Container>()) {
    auto container = getContainer();
    container->width = Container::Scale::Label::Pixel;
    container->height = Container::Scale::Label::Pixel;
}

void RootElement::render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                         const std::weak_ptr<CommandBuffer>& commandBuffer,
                         const std::shared_ptr<DrawPass>& drawPass, const std::shared_ptr<Texture>& background) {
    auto container = getContainer();
    container->x = 0;
    container->y = 0;
    container->width = screen.w;
    container->height = screen.h;
    container->compute(scale);
}
