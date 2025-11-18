#include <iostream>

#include "Events/Event.h"
#include "Framework/App.h"
#include "Platform/Window.h"
#include "Rendering/Display.h"
#include "Rendering/DrawPass.h"
#include "Rendering/Renderer.h"
#include "Rendering/Texture.h"
#include "UI/Theme.h"
#include "UI/UICompositor.h"
#include "UI/UILayer.h"
#include "UI/Elements/BlurPanel.h"
#include "UI/Elements/HorizontalLayout.h"
#include "UI/Elements/Panel.h"
#include "UI/Elements/Root.h"
#include "UI/Elements/VerticalLayout.h"

class MessageApp : public App {
public:
    std::shared_ptr<Window> window;
    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<Texture> target;
    std::shared_ptr<Display> display;

    std::shared_ptr<UILayer> layer1;
    std::shared_ptr<UILayer> layer2;
    std::shared_ptr<UICompositor> compositor;

    std::shared_ptr<Theme> theme;

    std::shared_ptr<Root> root;

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

        root = std::make_shared<Root>();

        layer1 = std::make_shared<UILayer>(renderer);

        //Create a vertical layout
        auto layout = std::make_shared<VerticalLayout>();
        auto layoutContainer = layout->getContainer();
        layoutContainer->x = {Container::Position::Label::Percent, 0.5f};
        layoutContainer->y = {Container::Position::Label::Percent, 0.5f};
        layoutContainer->xPivot = {Container::Position::Label::Percent, 0.5f};
        layoutContainer->yPivot = {Container::Position::Label::Percent, 0.5f};
        layoutContainer->width = {Container::Scale::Label::Pixel, 512.0f};
        layoutContainer->height = {Container::Scale::Label::Pixel, 512.0f};
        layoutContainer->padding = {Container::Scale::Label::Pixel, 8.0f};
        root->addChild(layout);

        auto menuBar = std::make_shared<HorizontalLayout>();
        auto menuBarPanel = std::make_shared<Panel>();
        menuBar->addChild(menuBarPanel);
        layout->addChild(menuBar);

        //Add a couple panel
        for (int i = 0; i < 8; i++) {
            auto subLayout = std::make_shared<HorizontalLayout>();
            for (int y = 0; y < 9; y++) {
                auto panel = std::make_shared<Panel>();
                if ((i + y) % 2 == 0) {
                    panel->fill = "accent-fill";
                    panel->border = "accent-border";
                } else {
                    panel->fill = "neutral-fill";
                    panel->border = "neutral-border";
                }
                subLayout->addChild(panel);
            }
            layout->addChild(subLayout);
        }

        layer1->addElement(root);

        layer2 = std::make_shared<UILayer>(renderer);

        auto blurRoot = std::make_shared<Root>();

        auto padding = std::make_shared<Panel>();

        padding->fill = "";
        padding->border = "";

        blurRoot->addChild(padding);

        auto blurPanel = std::make_shared<BlurPanel>();

        auto blurContainer = blurPanel->getContainer();
        blurContainer->x = {Container::Position::Label::Percent, 0.5f};
        blurContainer->y = {Container::Position::Label::Percent, 0.5f};
        blurContainer->width = {Container::Scale::Label::Pixel, 256.0f};
        blurContainer->height = {Container::Scale::Label::Pixel, 256.0f};
        blurContainer->xPivot = {Container::Position::Label::Percent, 0.5f};
        blurContainer->yPivot = {Container::Position::Label::Percent, 0.5f};

        padding->addChild(blurPanel);

        layer2->addElement(blurRoot);

        compositor = std::make_shared<UICompositor>(renderer);
        compositor->pushLayer(layer1);
        compositor->pushLayer(layer2);

        theme = Theme::dark();
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

        int width, height;
        window->getRealSize(width, height);

        Viewport viewport = {0, 0, 0, 0, 0.0f, 1.0f};
        viewport.x = 0;
        viewport.y = 0;
        viewport.w = static_cast<float>(width);
        viewport.h = static_cast<float>(height);

        auto cmd = renderer->begin();


        auto ui = compositor->render(viewport, window->getScale(), theme, cmd);

        display->target(cmd, ui);

        renderer->end();

        display->present();

        std::cout << 1.0f / delta << std::endl;
    }
};

int main() {
    MessageApp app;
    app.run();
    return 0;
}
