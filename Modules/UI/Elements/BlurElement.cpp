#include "BlurElement.h"

std::shared_ptr<Pipeline> BlurElement::glassPipeline = nullptr;
std::shared_ptr<Sampler> BlurElement::glassSampler = nullptr;

void BlurElement::onAddedToLayer(const std::shared_ptr<Renderer>& renderer) {
    if (glassPipeline == nullptr) {
        auto vertex = std::make_shared<Shader>(renderer, Shader::Type::Vertex, "Resources/Glass.hlsl", 0, 1, 0, 0);
        auto fragment = std::make_shared<Shader>(renderer, Shader::Type::Fragment, "Resources/Glass.hlsl", 1, 1, 0, 0);

        glassPipeline = std::make_shared<Pipeline>(renderer, Texture::Format::R8G8B8A8_SRGB);
        glassPipeline->vertex(vertex, {});
        glassPipeline->fragment(fragment);
        glassPipeline->primitive(Pipeline::PrimitiveType::TriangleList);
        glassPipeline->fill(Pipeline::FillMode::Fill);
        glassPipeline->build();
    }

    if (glassSampler == nullptr) {
        glassSampler = std::make_shared<Sampler>(renderer, Sampler::Filter::Nearest, Sampler::Wrap::Mirrored);
    }

    pipeline = glassPipeline;
}

void BlurElement::render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                       const std::weak_ptr<CommandBuffer>& commandBuffer, const std::shared_ptr<DrawPass>& drawPass,
                       const std::shared_ptr<Texture>& background) {
    pipeline->texture(0, background, glassSampler);
    PanelElement::render(screen, scale, theme, commandBuffer, drawPass, background);
}

