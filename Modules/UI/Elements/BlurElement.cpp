#include "BlurElement.h"

std::shared_ptr<Pipeline> BlurElement::glassPipeline = nullptr;
std::shared_ptr<Sampler> BlurElement::glassSampler = nullptr;

void BlurElement::init(const std::shared_ptr<Renderer>& renderer) {
    if (glassPipeline == nullptr) {
        auto vertex = Shader::make(renderer, Shader::Type::Vertex, "Resources/Shaders/Glass.hlsl", 0, 1, 0,
                                   0);
        auto fragment = Shader::make(renderer, Shader::Type::Fragment, "Resources/Shaders/Glass.hlsl", 1,
                                     1,
                                     0, 0);

        glassPipeline = Pipeline::make(renderer);
        glassPipeline->vertex(vertex, {});
        glassPipeline->fragment(fragment);
        glassPipeline->primitive(Pipeline::PrimitiveType::TriangleList);
        glassPipeline->fill(Pipeline::FillMode::Fill);
        glassPipeline->build();
    }

    if (glassSampler == nullptr) {
        glassSampler = Sampler::make(renderer, Sampler::Filter::Nearest, Sampler::Wrap::Mirrored);
    }
}

void BlurElement::onAddedToLayer(const std::shared_ptr<Renderer>& renderer) {
    init(renderer);
}

void BlurElement::render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                         const std::weak_ptr<CommandBuffer>& commandBuffer, const std::shared_ptr<DrawPass>& drawPass,
                         const std::shared_ptr<Texture>& background) {
    pipeline->texture(0, background, glassSampler);
    PanelElement::render(screen, scale, theme, commandBuffer, drawPass, background);
}

std::shared_ptr<BlurElement> BlurElement::make() {
    return std::shared_ptr<BlurElement>(new BlurElement());
}

BlurElement::BlurElement() = default;
