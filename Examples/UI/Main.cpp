#include <iostream>

#include "Events/Event.h"
#include "Framework/App.h"
#include "Platform/Window.h"
#include "Rendering/Display.h"
#include "Rendering/DrawPass.h"
#include "Rendering/Renderer.h"
#include "Rendering/Texture.h"
#include "UI/UICompositor.h"
#include "UI/UILayer.h"
#include "UI/Elements/Panel.h"
#include "UI/Elements/VerticalLayout.h"

class UIExample : public App {
public:
    std::shared_ptr<Window> window;
    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<Texture> target;
    std::shared_ptr<Display> display;

    std::shared_ptr<UILayer> layer;
    std::shared_ptr<UICompositor> compositor;

    std::shared_ptr<Container> root;

    void start() override {
        window = Window::create();
        window->setTitle("UI Example");
        window->setSize(1024, 720);

        window->open();


        window->setFlag(Window::Resizable, true);
        window->setFlag(Window::Transparent, true);
        window->setFlag(Window::Blur, true);
        window->setFlag(Window::HighDPI, true);

        renderer = std::make_shared<Renderer>();
        target = std::make_shared<Texture>(renderer, Texture::Format::R8G8B8A8_SRGB, 1024, 720, 1,
                                           BitFlag<Texture::Flags>({Texture::Flags::RenderTarget}));

        display = std::make_shared<Display>(renderer, window);

        root = std::make_shared<Container>();

        layer = std::make_shared<UILayer>(renderer);

        //Create a vertical layout
        auto vLayout = std::make_shared<VerticalLayout>();
        root->appendChild(vLayout->getContainer());

        //Add a couple panel
        for (int i = 0; i < 3; i++) {
            auto panel = std::make_shared<Panel>();
            vLayout->addChild(panel);
        }

        layer->addElement(vLayout);

        compositor = std::make_shared<UICompositor>(renderer);
        compositor->pushLayer(layer);
    }

    void update(float delta) override {
        Event event;
        while (Event::getEvent(event)) {
            if (auto* close = event.as<WindowCloseEvent>()) {
                quit();
            }
            if (auto* resize = event.as<WindowResizeEvent>()) {
                target = std::make_shared<Texture>(renderer, Texture::Format::R8G8B8A8_SRGB, resize->realWidth,
                                                   resize->realHeight, 1,
                                                   BitFlag<Texture::Flags>({Texture::Flags::RenderTarget}));
            }
        }

        int windowWidth, windowHeight;
        window->getSize(windowWidth, windowHeight);

        root->x = 0;
        root->y = 0;
        root->width = static_cast<float>(windowWidth);
        root->height = static_cast<float>(windowHeight);

        root->computeChildren();

        auto cmd = renderer->begin();

        Viewport viewport = {0, 0, 0, 0, 0.0f, 1.0f};
        int width, height;
        window->getSize(width, height);
        viewport.x = 0;
        viewport.y = 0;
        viewport.w = static_cast<float>(width);
        viewport.h = static_cast<float>(height);

        auto ui = compositor->render(viewport, cmd);

        display->target(cmd, ui);

        renderer->end();

        display->present();
    }
};

int main() {
    UIExample app;
    app.run();
    return 0;
}
