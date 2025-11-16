#include "Panel.h"

#include <iostream>

Panel::Panel() : UIElement(std::make_shared<Container>()) {
    auto container = getContainer();
}

void Panel::onAddedToLayer(const std::shared_ptr<Renderer>& renderer) {
    auto vertex = std::make_shared<Shader>(renderer, Shader::Type::Vertex, "Resources/Panel.hlsl", 0, 1, 0, 0);
    auto fragment = std::make_shared<Shader>(renderer, Shader::Type::Fragment, "Resources/Panel.hlsl", 0, 1, 0, 0);

    this->pipeline = std::make_shared<Pipeline>(renderer, Texture::Format::R8G8B8A8_SRGB);
    this->pipeline->vertex(vertex, {});
    this->pipeline->fragment(fragment);
    this->pipeline->primitive(Pipeline::PrimitiveType::TriangleList);
    this->pipeline->fill(Pipeline::FillMode::Fill);
    this->pipeline->build();
}

void Panel::render(Viewport screen, const std::weak_ptr<CommandBuffer>& commandBuffer,
                   const std::shared_ptr<DrawPass>& drawPass,
                   const std::shared_ptr<Texture>& background) {
    auto containerRect = getContainer()->real();
    PanelUniform uniform = {
        -screen.w / 2.0f, -screen.h / 2.0f, screen.w, screen.h,
        containerRect.x, containerRect.y, containerRect.width, containerRect.height
    };

    this->pipeline->bind(commandBuffer, drawPass);
    this->pipeline->uniform(Shader::Type::Vertex, 0, uniform);
    this->pipeline->uniform(Shader::Type::Fragment, 0, uniform);

    drawPass->drawPrimitives(6, 0, 1, 0);
}
