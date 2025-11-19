#include "UICompositor.h"

#include <iostream>
#include <ostream>

UICompositor::UICompositor(const std::shared_ptr<Renderer>& renderer) {
    this->renderer = renderer;

    //Load/create composite shader pipeline

    auto vertex = std::make_shared<Shader>(renderer, Shader::Type::Vertex, "Resources/Compositor.hlsl", 0, 0, 0, 0);
    auto fragment = std::make_shared<Shader>(renderer, Shader::Type::Fragment, "Resources/Compositor.hlsl", 2, 0, 0, 0);

    this->pipeline = std::make_shared<Pipeline>(renderer, Texture::Format::R8G8B8A8_SRGB);
    this->pipeline->vertex(vertex, {});
    this->pipeline->fragment(fragment);
    this->pipeline->primitive(Pipeline::PrimitiveType::TriangleList);
    this->pipeline->fill(Pipeline::FillMode::Fill);
    this->pipeline->build();

    this->sampler = std::make_shared<Sampler>(renderer, Sampler::Filter::Nearest, Sampler::Wrap::Clamp);

    this->layers = {};
}

void UICompositor::addLayer(const std::shared_ptr<UILayer>& layer) {
    this->layers.push_back(layer);
}

std::shared_ptr<Texture> UICompositor::render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                                              const std::weak_ptr<CommandBuffer>& commandBuffer) {
    if (ping == nullptr || ping->width() != static_cast<int>(screen.w) || ping->height() !=
        static_cast<int>(screen.h)) {
        ping = std::make_shared<Texture>(renderer, Texture::Format::R8G8B8A8_SRGB, screen.w, screen.h, 1,
                                         BitFlag<Texture::Flags>({Texture::Flags::RenderTarget}));

        pong = std::make_shared<Texture>(renderer, Texture::Format::R8G8B8A8_SRGB, screen.w, screen.h, 1,
                                         BitFlag<Texture::Flags>({Texture::Flags::RenderTarget}));
    }

    //Clear composited
    auto clearPass = std::make_shared<DrawPass>(renderer);
    clearPass->texture(ping);
    clearPass->clear({0.0f, 0.0f, 0.0f, 0.0f});
    clearPass->viewport(screen);
    clearPass->begin(commandBuffer);
    clearPass->end();

    if (layers.empty())
        return ping;

    auto compositePass = std::make_shared<DrawPass>(renderer);
    compositePass->clear({0.0f, 0.0f, 0.0f, 0.0f});
    compositePass->viewport(screen);

    for (const auto& i: layers) {
        auto layer = i->render(screen, scale, theme, commandBuffer, ping);

        //Render to pong
        compositePass->texture(pong);
        compositePass->begin(commandBuffer);

        pipeline->texture(0, ping, sampler);
        pipeline->texture(1, layer, sampler);
        pipeline->bind(commandBuffer, compositePass);

        compositePass->drawPrimitives(6, 0, 1, 0);

        compositePass->end();

        std::swap(ping, pong); //Ping pong swap
    }

    return ping;
}

void UICompositor::pushEvent(Event& event) const {
    for (auto& layer: layers) {
        layer->pushEvent(event);
    }
}
