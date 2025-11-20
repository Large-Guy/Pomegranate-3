#include "PanelElement.h"

#include <iostream>

#include "UI/Theme.h"

std::shared_ptr<Pipeline> PanelElement::standardPipeline = nullptr;

PanelElement::PanelElement() : UIElement(std::make_shared<Container>()) {
    auto container = getContainer();
}

void PanelElement::onAddedToLayer(const std::shared_ptr<Renderer>& renderer) {
    if (standardPipeline == nullptr) {
        auto vertex = std::make_shared<Shader>(renderer, Shader::Type::Vertex, "Resources/Shaders/Panel.hlsl", 0,
                                               1, 0, 0);
        auto fragment = std::make_shared<Shader>(renderer, Shader::Type::Fragment, "Resources/Shaders/Panel.hlsl", 0,
                                                 1,
                                                 0, 0);

        standardPipeline = std::make_shared<Pipeline>(renderer, Texture::Format::R8G8B8A8_SRGB);
        standardPipeline->vertex(vertex, {});
        standardPipeline->fragment(fragment);
        standardPipeline->primitive(Pipeline::PrimitiveType::TriangleList);
        standardPipeline->fill(Pipeline::FillMode::Fill);
        standardPipeline->build();
    }

    pipeline = standardPipeline;
}

void PanelElement::render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                          const std::weak_ptr<CommandBuffer>& commandBuffer,
                          const std::shared_ptr<DrawPass>& drawPass,
                          const std::shared_ptr<Texture>& background) {
    auto fillColor = theme->get<Color>(fill);
    auto borderColor = theme->get<Color>(border);
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
