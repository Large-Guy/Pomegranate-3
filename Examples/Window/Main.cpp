#include <iostream>
#include<Platform/Window.h>
#include<SDL3/SDL.h>

#include "Events/Event.h"
#include "Framework/App.h"
#include "Rendering/Renderer.h"
#include "Rendering/RenderPass.h"
#include "Rendering/Display.h"
#include "Rendering/DrawPass.h"
#include "Rendering/Pipeline.h"
#include "Rendering/Texture.h"

class WindowExample : public App {
public:
    std::shared_ptr<Window> window;
    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<Texture> target;
    std::shared_ptr<Display> display;
    std::shared_ptr<Pipeline> pipeline;

    void start() override {
        window = Window::create();
        renderer = std::make_shared<Renderer>();
        target = std::make_shared<Texture>(renderer, Texture::Format::R8G8B8A8_SRGB, 800, 600,
                                           BitFlag<Texture::Flags>({Texture::Flags::RenderTarget}));

        window->setTitle("Window Example");
        window->open();

        window->setFlag(Window::Resizable, true);
        window->setFlag(Window::Transparent, true);
        window->setFlag(Window::Blur, true);
        window->setFlag(Window::HighDPI, true);
        window->setFlag(Window::Full, true);

        window->setBlurRadius(5);

        display = std::make_shared<Display>(renderer, window);

        std::shared_ptr<Shader> vertex = std::make_shared<Shader>(renderer, Shader::Type::Vertex,
                                                                  "Resources/Triangle.hlsl", 0, 0, 0, 0);
        std::shared_ptr<Shader> fragment = std::make_shared<Shader>(renderer, Shader::Type::Fragment,
                                                                    "Resources/Triangle.hlsl", 0, 0, 0, 0);

        pipeline = std::make_shared<Pipeline>(renderer, Texture::Format::R8G8B8A8_SRGB);
        pipeline->vertex(vertex);
        pipeline->fragment(fragment);
        pipeline->build();
    }

    void update(float delta) override {
        Event event;
        while (Event::getEvent(event)) {
            if (auto* close = event.as<WindowCloseEvent>()) {
                quit();
            }
        }

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

        pipeline->bind(cmdQueue, draw);
        SDL_DrawGPUPrimitives(static_cast<SDL_GPURenderPass*>(draw->getInternal()), 3, 1, 0, 0);

        draw->end();

        display->target(cmdQueue, target);

        renderer->end();

        display->present();
    }
};

int main() {
    std::shared_ptr<WindowExample> app = std::make_shared<WindowExample>();

    app->run();

    return 0;
}
