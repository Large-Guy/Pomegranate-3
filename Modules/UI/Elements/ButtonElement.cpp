#include "ButtonElement.h"

ButtonElement::ButtonElement() {
}

void ButtonElement::onPressed(MouseEvent::Button button) {
    onPress();
}

void ButtonElement::onReleased(MouseEvent::Button button) {
    onRelease();
}


void ButtonElement::render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                           const std::weak_ptr<CommandBuffer>& commandBuffer, const std::shared_ptr<DrawPass>& drawPass,
                           const std::shared_ptr<Texture>& background) {
    PanelElement::render(screen, scale, theme, commandBuffer, drawPass, background);
    auto fillColor = theme->get<Color>(fill);
    auto borderColor = theme->get<Color>(border);
    if (isHovered()) {
        borderColor += {0.15f, 0.15f, 0.15f, 0.0f};
    }
    auto containerRect = getContainer()->real();
    PanelUniform uniform = {
        -screen.w / 2.0f, -screen.h / 2.0f, screen.w, screen.h,
        containerRect.x, containerRect.y, containerRect.width, containerRect.height,
        fillColor,
        borderColor
    };

    this->pipeline->bind(commandBuffer, drawPass);
    this->pipeline->uniform(Shader::Type::Vertex, 0, uniform);
    this->pipeline->uniform(Shader::Type::Fragment, 0, uniform);

    drawPass->drawPrimitives(6, 0, 1, 0);
}
