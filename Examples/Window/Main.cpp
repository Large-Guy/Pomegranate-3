#include <iostream>
#include<Platform/Window.h>

#include "Events/Event.h"
#include "Framework/App.h"
#include "Layout/Container.h"
#include "Layout/Horizontal.h"
#include "Layout/Vertical.h"
#include "Rendering/Buffer.h"
#include "Rendering/CopyPass.h"
#include "Rendering/Renderer.h"
#include "Rendering/RenderPass.h"
#include "Rendering/Display.h"
#include "Rendering/DrawPass.h"
#include "Rendering/Pipeline.h"
#include "Rendering/Texture.h"

struct Vertex {
    float x, y, z;
    float r, g, b, a;
};


struct alignas(16) PanelUniform {
    float screenX, screenY, screenW, screenH;
    float rectX, rectY, rectW, rectH;
};

#define UI_LAYERS 2

class WindowExample : public App {
public:
    std::shared_ptr<Window> window;
    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<Display> display;
    std::shared_ptr<Buffer> buffer;

    std::shared_ptr<Texture> target;
    std::shared_ptr<Texture> layers[UI_LAYERS];

    std::shared_ptr<Sampler> sampler;

    std::shared_ptr<Container> container;
    std::shared_ptr<Vertical> sidebarContents;

    std::shared_ptr<Pipeline> panelPipeline;
    std::shared_ptr<Pipeline> glassPipeline;
    std::shared_ptr<Pipeline> compositorPipeline;

    float time = 0.0;

    void start() override {
        window = Window::create();
        window->setSize(1024, 720);
        window->setTitle("Window Example");
        window->open();

        window->setFlag(Window::Resizable, true);
        window->setFlag(Window::Transparent, true);
        window->setFlag(Window::Blur, true);
        window->setFlag(Window::HighDPI, true);
        //window->setFlag(Window::Full, true);

        renderer = std::make_shared<Renderer>();

        int width, height;
        window->getRealSize(width, height);

        sampler = std::make_shared<Sampler>(renderer, Sampler::Filter::Nearest, Sampler::Wrap::Clamp);

        target = std::make_shared<Texture>(renderer, Texture::Format::R8G8B8A8_SRGB, width, height, 1,
                                           BitFlag<Texture::Flags>({Texture::Flags::RenderTarget}));
        for (auto& layer: layers)
            layer = std::make_shared<Texture>(renderer, Texture::Format::R8G8B8A8_SRGB, width, height, 1,
                                              BitFlag<Texture::Flags>({Texture::Flags::RenderTarget}));


        window->setBlurRadius(5);

        display = std::make_shared<Display>(renderer, window);

        std::shared_ptr<Shader> panelVertexShader = std::make_shared<Shader>(renderer, Shader::Type::Vertex,
                                                                             "Resources/Panel.hlsl", 0, 1, 0, 0);
        std::shared_ptr<Shader> panelFragmentShader = std::make_shared<Shader>(renderer, Shader::Type::Fragment,
                                                                               "Resources/Panel.hlsl", 0, 1, 0, 0);

        panelPipeline = std::make_shared<Pipeline>(renderer, Texture::Format::R8G8B8A8_SRGB);
        panelPipeline->vertex(panelVertexShader, {});
        panelPipeline->fragment(panelFragmentShader);
        panelPipeline->primitive(Pipeline::PrimitiveType::TriangleList);
        panelPipeline->fill(Pipeline::FillMode::Fill);
        panelPipeline->build();

        std::shared_ptr<Shader> glassVertexShader = std::make_shared<Shader>(
            renderer, Shader::Type::Vertex, "Resources/Glass.hlsl", 0, 1, 0, 0);
        std::shared_ptr<Shader> glassFragmentShader = std::make_shared<Shader>(
            renderer, Shader::Type::Fragment, "Resources/Glass.hlsl", 1, 1, 0, 0);

        glassPipeline = std::make_shared<Pipeline>(renderer, Texture::Format::R8G8B8A8_SRGB);
        glassPipeline->vertex(glassVertexShader, {});
        glassPipeline->fragment(glassFragmentShader);
        glassPipeline->primitive(Pipeline::PrimitiveType::TriangleList);
        glassPipeline->fill(Pipeline::FillMode::Fill);
        glassPipeline->build();

        std::shared_ptr<Shader> compositorVertexShader = std::make_shared<Shader>(
            renderer, Shader::Type::Vertex, "Resources/Compositor.hlsl", 0, 0, 0, 0);
        std::shared_ptr<Shader> compositorFragmentShader = std::make_shared<Shader>(
            renderer, Shader::Type::Fragment, "Resources/Compositor.hlsl", 2, 0, 0, 0);

        compositorPipeline = std::make_shared<Pipeline>(renderer, Texture::Format::R8G8B8A8_SRGB);
        compositorPipeline->vertex(compositorVertexShader, {});
        compositorPipeline->fragment(compositorFragmentShader);
        compositorPipeline->primitive(Pipeline::PrimitiveType::TriangleList);
        compositorPipeline->fill(Pipeline::FillMode::Fill);
        compositorPipeline->build();

        buffer = std::make_shared<Buffer>(renderer, Buffer::Type::Vertex, sizeof(Vertex) * 3);

        auto upload = std::make_shared<TransferBuffer>(renderer, TransferBuffer::Type::Upload, sizeof(Vertex) * 3);

        auto* uploadData = upload->map<Vertex>();
        uploadData[0] = {
            .x = -1.0f,
            .y = -1.0f,
            .z = 0.0f,
            .r = 1.0f,
            .g = 0.0f,
            .b = 0.0f,
            .a = 1.0f
        };
        uploadData[1] = {
            .x = 1.0f,
            .y = -1.0f,
            .z = 0.0f,
            .r = 0.0f,
            .g = 1.0f,
            .b = 0.0f,
            .a = 1.0f
        };
        uploadData[2] = {
            .x = 0.0f,
            .y = 1.0f,
            .z = 0.0f,
            .r = 0.0f,
            .g = 0.0f,
            .b = 1.0f,
            .a = 0.0f
        };
        upload->unmap();

        auto commandQueue = renderer->begin();
        std::shared_ptr<CopyPass> copyPass = std::make_shared<CopyPass>(renderer);
        copyPass->begin(commandQueue);

        buffer->upload(copyPass, upload);

        copyPass->end();
        renderer->end();

        container = std::make_shared<Vertical>();
        container->x = {Container::Position::Label::Physical, 0.f};
        container->y = {Container::Position::Label::Physical, 0.f};
        container->width = {Container::Scale::Label::Physical, 1.0f};
        container->height = {Container::Scale::Label::Physical, 1.0f};

        //Menu bar
        container->appendChild(std::make_shared<Horizontal>());
        container->getChild(0)->height = {Container::Scale::Label::Pixel, 30.0f};

        container->appendChild(std::make_shared<Horizontal>());


        auto body = container->getChild(1);

        //Sidebar
        body->appendChild(std::make_shared<Horizontal>());

        body->getChild(0)->width = {Container::Scale::Label::Pixel, 256.0f};
        body->getChild(0)->height = {Container::Scale::Label::Percent, 1.0f};

        body->appendChild(std::make_shared<Container>());

        auto sidebar = body->getChild<Horizontal>(0);

        auto scrollbar = std::make_shared<Container>();
        scrollbar->width = {Container::Scale::Label::Pixel, 12.0f};

        auto handle = std::make_shared<Container>();
        handle->height = {Container::Scale::Label::Pixel, 64.0f};
        scrollbar->appendChild(handle);

        sidebar->appendChild(scrollbar);

        sidebarContents = std::make_shared<Vertical>();
        sidebarContents->padding = {Container::Scale::Label::Pixel, 8.0f};
        sidebarContents->gap = {Container::Scale::Label::Pixel, 8.0f};
        sidebarContents->yAlignment = Flexable::Alignment::Leading;
        sidebarContents->yOffset = Container::Position::Label::Pixel;
        sidebar->appendChild(sidebarContents);

        for (int i = 0; i < 16; i++) {
            auto bar = std::make_shared<Container>();

            bar->x = {Container::Position::Label::Pixel, 16};
            bar->y = {Container::Position::Label::Pixel, 16};
            bar->width = {Container::Scale::Label::Auto, 0.0f};
            bar->height = {Container::Scale::Label::Pixel, 32.0f};

            sidebarContents->appendChild(bar);
        }
    }

    void drawRectangle(const std::weak_ptr<CommandQueue>& cmd, const std::shared_ptr<DrawPass>& pass, float x, float y,
                       float w, float h) const {
        int width, height;
        window->getSize(width, height);
        PanelUniform panelUniform{
            .screenX = static_cast<float>(width) / -2.0f,
            .screenY = static_cast<float>(height) / -2.0f,
            .screenW = static_cast<float>(width),
            .screenH = static_cast<float>(height),
            .rectX = x,
            .rectY = y,
            .rectW = w,
            .rectH = h
        };
        panelPipeline->uniform(Shader::Type::Vertex, 0, panelUniform);
        panelPipeline->uniform(Shader::Type::Fragment, 0, panelUniform);
        pass->drawPrimitives(6, 0, 1, 0);
    }

    void drawContainer(const std::weak_ptr<CommandQueue>& cmd, const std::shared_ptr<DrawPass>& pass,
                       const std::shared_ptr<Container>& con) {
        auto [x, y, width, height] = con->real();
        panelPipeline->bind(cmd, pass);
        drawRectangle(cmd, pass, x, y, width, height);
        for (auto& child: *con) {
            pass->scissor(x * 2.0f + 1.0f, y * 2.0f + 1.0f, width * 2.0f - 2.0f, height * 2.0f - 2.0f);
            drawContainer(cmd, pass, child);
        }
    }

    void update(float delta) override {
        time += delta;

        Event event;
        while (Event::getEvent(event)) {
            if (auto* close = event.as<WindowCloseEvent>()) {
                quit();
            }
            if (auto* resize = event.as<WindowResizeEvent>()) {
                target = std::make_shared<Texture>(renderer, Texture::Format::R8G8B8A8_SRGB, resize->realWidth,
                                                   resize->realHeight, 1,
                                                   BitFlag<Texture::Flags>({Texture::Flags::RenderTarget}));
                for (auto& layer: layers)
                    layer = std::make_shared<Texture>(renderer, Texture::Format::R8G8B8A8_SRGB, resize->realWidth,
                                                      resize->realHeight, 1,
                                                      BitFlag<Texture::Flags>({Texture::Flags::RenderTarget}));
            }
        }

        int width, height;
        window->getSize(width, height);

        container->width = static_cast<float>(width);
        container->height = static_cast<float>(height);

        sidebarContents->yOffset = sinf(time * 0.1f) * 360;

        container->computeChildren();

        auto cmdQueue = renderer->begin();

        auto draw = std::make_shared<DrawPass>(renderer);

        //Panels

        draw->texture(layers[0], 0);

        draw->clear({0.0f, 0.0f, 0.0f, 0.0f});

        draw->viewport({
            .x = 0.0f,
            .y = 0.0f,
            .w = static_cast<float>(target->width()),
            .h = static_cast<float>(target->height())
        });

        draw->begin(cmdQueue);

        drawContainer(cmdQueue, draw, container);

        draw->end();

        //Glass layer

        draw->clear({0.0f, 0.0f, 0.0f, 0.0f});

        draw->texture(layers[1], 0);

        draw->begin(cmdQueue);

        glassPipeline->texture(0, layers[0], sampler);
        glassPipeline->bind(cmdQueue, draw);

        float padding = 64.0f;

        drawRectangle(cmdQueue, draw, padding, padding, static_cast<float>(width) - padding * 2.0f,
                      static_cast<float>(height) - padding * 2.0f);

        draw->end();

        //Compositor

        draw->clear({0.0f, 0.0f, 0.0f, 0.33f});

        draw->texture(target, 0);

        draw->begin(cmdQueue);

        compositorPipeline->texture(0, layers[0], sampler);
        compositorPipeline->texture(1, layers[1], sampler);
        compositorPipeline->bind(cmdQueue, draw);

        draw->drawPrimitives(6, 0, 1, 0);

        draw->end();

        display->target(cmdQueue, target);

        renderer->end();

        display->present();

        //std::cout << "Framerate: " << 1.0f / delta << std::endl;
    }
};

int main() {
    std::shared_ptr<WindowExample> app = std::make_shared<WindowExample>();

    app->run();

    return 0;
}
