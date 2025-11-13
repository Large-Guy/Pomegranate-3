#include <iostream>
#include<Platform/Window.h>

#include "Events/Event.h"
#include "Framework/App.h"
#include "Layout/Container.h"
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


class WindowExample : public App {
public:
    std::shared_ptr<Window> window;
    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<Texture> target;
    std::shared_ptr<Display> display;
    std::shared_ptr<Pipeline> pipeline;
    std::shared_ptr<Buffer> buffer;

    std::shared_ptr<Container> container;

    void start() override {
        window = Window::create();
        window->setSize(800, 600);
        window->setTitle("Window Example");
        window->open();

        window->setFlag(Window::Resizable, true);
        window->setFlag(Window::Transparent, true);
        window->setFlag(Window::Blur, true);
        window->setFlag(Window::HighDPI, true);
        window->setFlag(Window::Full, true);

        renderer = std::make_shared<Renderer>();

        int width, height;
        window->getRealSize(width, height);
        target = std::make_shared<Texture>(renderer, Texture::Format::R8G8B8A8_SRGB, width, height,
                                           BitFlag<Texture::Flags>({Texture::Flags::RenderTarget}));


        window->setBlurRadius(5);

        display = std::make_shared<Display>(renderer, window);

        std::shared_ptr<Shader> vertex = std::make_shared<Shader>(renderer, Shader::Type::Vertex,
                                                                  "Resources/Panel.hlsl", 0, 1, 0, 0);
        std::shared_ptr<Shader> fragment = std::make_shared<Shader>(renderer, Shader::Type::Fragment,
                                                                    "Resources/Panel.hlsl", 0, 1, 0, 0);

        pipeline = std::make_shared<Pipeline>(renderer, Texture::Format::R8G8B8A8_SRGB);
        pipeline->vertex(vertex, {
                             .slot = 0,
                             .size = sizeof(Vertex),
                             .attributes = {
                                 {
                                     .bufferSlot = 0,
                                     .type = VertexDescription::Attribute::Type::float3,
                                     .location = 0,
                                     .offset = 0
                                 },
                                 {
                                     .bufferSlot = 0,
                                     .type = VertexDescription::Attribute::Type::float4,
                                     .location = 1,
                                     .offset = offsetof(Vertex, r)
                                 }
                             }
                         });
        pipeline->fragment(fragment);
        pipeline->primitive(Pipeline::PrimitiveType::TriangleList);
        pipeline->fill(Pipeline::FillMode::Fill);
        pipeline->build();

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

        container = std::make_shared<Container>(Container{
            Container::Type::Vertical,
            {Container::Position::Label::Physical, 0.f},
            {Container::Position::Label::Physical, 0.f},
            {Container::Scale::Label::Physical, 1.0f},
            {Container::Scale::Label::Physical, 1.0f}
        });

        container->appendChild(std::make_shared<Container>(Container{
            Container::Type::Horizontal,
            {Container::Position::Label::Auto, 0.f},
            {Container::Position::Label::Auto, 0.f},
            {Container::Scale::Label::Auto, 0.0f},
            {Container::Scale::Label::Auto, 0.0f}
        }));

        //Menu bar
        container->appendChild(std::make_shared<Container>(Container{
            Container::Type::Horizontal,
            {Container::Position::Label::Auto, 0.f},
            {Container::Position::Label::Auto, 0.f},
            {Container::Scale::Label::Auto, 0.0f},
            {Container::Scale::Label::Pixel, 30.0f}
        }));

        auto body = container->getChild(0);

        body->appendChild(std::make_shared<Container>(Container{
            Container::Type::Vertical,
            {Container::Position::Label::Pixel, 0},
            {Container::Position::Label::Pixel, 0},
            {Container::Scale::Label::Pixel, 256.0f},
            {Container::Scale::Label::Percent, 1.0f}
        }));

        body->appendChild(std::make_shared<Container>(Container{
            Container::Type::Float,
            {Container::Position::Label::Pixel, 0},
            {Container::Position::Label::Pixel, 0},
            {Container::Scale::Label::Auto, 0.0f},
            {Container::Scale::Label::Auto, 0.0f}
        }));

        auto sidebar = body->getChild(0);
        sidebar->padding = {Container::Scale::Label::Pixel, 8.0f};
        sidebar->gap = {Container::Scale::Label::Pixel, 8.0f};
        sidebar->alignment = Container::Alignment::Trailing;

        for (int i = 0; i < 8; i++) {
            sidebar->appendChild(std::make_shared<Container>(Container{
                Container::Type::Float,
                {Container::Position::Label::Pixel, 16},
                {Container::Position::Label::Pixel, 16},
                {Container::Scale::Label::Auto, 0.0f},
                {Container::Scale::Label::Pixel, 32.0f}
            }));
        }
    }

    void drawRectangle(const std::weak_ptr<CommandQueue>& cmd, const std::shared_ptr<DrawPass>& pass, float x, float y,
                       float w, float h) const {
        pipeline->bind(cmd, pass);
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
        pipeline->uniform(Shader::Type::Vertex, 0, panelUniform);
        pipeline->uniform(Shader::Type::Fragment, 0, panelUniform);
        pass->drawPrimitives(6, 0, 1, 0);
    }

    void drawContainer(const std::weak_ptr<CommandQueue>& cmd, const std::shared_ptr<DrawPass>& pass,
                       const std::shared_ptr<Container>& con) {
        auto [x, y, width, height] = con->real();
        drawRectangle(cmd, pass, x, y, width, height);
        for (int i = 0; i < con->getChildCount(); i++) {
            drawContainer(cmd, pass, con->getChild(i));
        }
    }

    void update(float delta) override {
        Event event;
        while (Event::getEvent(event)) {
            if (auto* close = event.as<WindowCloseEvent>()) {
                quit();
            }
            if (auto* resize = event.as<WindowResizeEvent>()) {
                target = std::make_shared<Texture>(renderer, Texture::Format::R8G8B8A8_SRGB, resize->realWidth,
                                                   resize->realHeight,
                                                   BitFlag<Texture::Flags>({Texture::Flags::RenderTarget}));
            }
        }

        int width, height;
        window->getSize(width, height);

        container->width = static_cast<float>(width);
        container->height = static_cast<float>(height);

        container->computeChildren();

        auto cmdQueue = renderer->begin();

        auto draw = std::make_shared<DrawPass>(renderer);
        draw->texture(target);
        draw->clear({0.0f, 0.0f, 0.0f, 0.33f});
        draw->viewport({
            .x = 0.0f,
            .y = 0.0f,
            .w = static_cast<float>(target->width()),
            .h = static_cast<float>(target->height())
        });
        draw->begin(cmdQueue);

        drawContainer(cmdQueue, draw, container);

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
