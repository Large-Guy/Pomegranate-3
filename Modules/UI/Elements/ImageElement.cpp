#include "ImageElement.h"

std::shared_ptr<Pipeline> ImageElement::standardPipeline = nullptr;

ImageElement::ImageElement() : UIElement(Container::make()) {
}

void ImageElement::onAddedToLayer(const std::shared_ptr<Renderer>& renderer) {
    if (standardPipeline == nullptr) {
        auto vertex = Shader::make(renderer, Shader::Type::Vertex, "Resources/Shaders///Texture.hlsl", 0,
                                   1, 0, 0);
        auto fragment = Shader::make(renderer, Shader::Type::Fragment, "Resources/Shaders///Texture.hlsl",
                                     1,
                                     1,
                                     0, 0);

        standardPipeline = Pipeline::make(renderer);
        standardPipeline->vertex(vertex, {});
        standardPipeline->fragment(fragment);
        standardPipeline->primitive(Pipeline::PrimitiveType::TriangleList);
        standardPipeline->fill(Pipeline::FillMode::Fill);
        standardPipeline->build();
    }

    pipeline = standardPipeline;
    sampler = Sampler::make(renderer, Sampler::Filter::Linear, Sampler::Wrap::Clamp);
}

void ImageElement::render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                          const std::weak_ptr<CommandBuffer>& commandBuffer, const std::shared_ptr<DrawPass>& drawPass,
                          const std::shared_ptr<Texture>& background) {
    if (image == nullptr)
        return;
    auto containerRect = getContainer()->real();
    Uniform uniform = {
        -screen.w / 2.0f, -screen.h / 2.0f, screen.w, screen.h,
        containerRect.x, containerRect.y, containerRect.width, containerRect.height
    };

    this->pipeline->texture(0, image, sampler);
    this->pipeline->bind(commandBuffer, drawPass);
    this->pipeline->uniform(Shader::Type::Vertex, 0, uniform);
    this->pipeline->uniform(Shader::Type::Fragment, 0, uniform);

    drawPass->drawPrimitives(6, 0, 1, 0);
}

std::shared_ptr<ImageElement> ImageElement::make() {
    return std::shared_ptr<ImageElement>(new ImageElement());
}
