#include <iostream>

#include "Events/Event.h"
#include "Framework/App.h"
#include "Layout/Fill.h"
#include "Layout/Flexable.h"
#include "Layout/Vertical.h"
#include "Platform/Window.h"
#include "Rendering/Display.h"
#include "Rendering/DrawPass.h"
#include "Rendering/Renderer.h"
#include "Rendering/Texture.h"
#include "UI/Theme.h"
#include "UI/UICompositor.h"
#include "UI/UILayer.h"
#include "UI/Elements/BlurElement.h"
#include "UI/Elements/ButtonElement.h"
#include "UI/Elements/FullLayoutElement.h"
#include "UI/Elements/HorizontalLayoutElement.h"
#include "UI/Elements/ImageElement.h"
#include "UI/Elements/PanelElement.h"
#include "UI/Elements/RootElement.h"
#include "UI/Elements/SpacerElement.h"
#include "UI/Elements/InputElement.h"
#include "UI/Elements/VerticalLayoutElement.h"

class MessageApp : public App {
public:
    std::shared_ptr<Window> window;
    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<Texture> target;
    std::shared_ptr<Display> display;

    std::shared_ptr<UICompositor> compositor;

    std::shared_ptr<Theme> theme;

    void initRendering() {
        window = Window::create();
        window->setTitle("Messaging App");
        window->setSize(1024, 720);

        window->open();


        window->setFlag(Window::Resizable, true);
        window->setFlag(Window::Transparent, true);
        window->setFlag(Window::Blur, true);
        window->setFlag(Window::Full, true);
        window->setFlag(Window::HighDPI, true);

        renderer = std::make_shared<Renderer>();
        target = std::make_shared<Texture>(renderer, Texture::Format::R8G8B8A8_SRGB, 1024, 720, 1,
                                           BitFlag<Texture::Flags>({Texture::Flags::RenderTarget}));

        display = std::make_shared<Display>(renderer, window);
    }

    void buildUI() {
        theme = Theme::dark();
        compositor = std::make_shared<UICompositor>(renderer);

        auto root = std::make_shared<RootElement>();

        //Split the screen vertically
        auto split = std::make_shared<HorizontalLayoutElement>();
        root->addChild(split);
        split->getContainer<Flexable>()->xFill = true;
        split->getContainer<Flexable>()->xOverflow = false;
        split->getContainer<Flexable>()->gap = 0;

        //People
        auto contacts = std::make_shared<PanelElement>();
        split->addChild(contacts);
        contacts->getContainer()->width = {Container::Scale::Label::Pixel, 256.0f};

        //Your messages with the selected chatter
        auto messages = std::make_shared<PanelElement>();
        split->addChild(messages);
        messages->getContainer()->width = {Container::Scale::Label::Auto, 0.0f};
        messages->fill = "secondary-fill";
        messages->border = "secondary-border";

        auto messageSplit = std::make_shared<VerticalLayoutElement>();
        messageSplit->getContainer()->gap = {Container::Scale::Label::Pixel, 0.0f};
        messageSplit->getContainer<Vertical>()->yOverflow = false;
        messages->addChild(messageSplit);
        auto messageSpacer = std::make_shared<FullLayoutElement>();
        messageSplit->addChild(messageSpacer);

        auto messageContents = std::make_shared<VerticalLayoutElement>();
        messageSpacer->addChild(messageContents);

        //Add some text
        auto text = std::make_shared<TextElement>();
        messageContents->addChild(text);

        auto messageBox = std::make_shared<FullLayoutElement>();
        messageBox->getContainer()->height = {Container::Scale::Label::Pixel, 64.0f};

        auto messageHorizontalLayout = std::make_shared<HorizontalLayoutElement>();
        messageBox->addChild(messageHorizontalLayout);

        auto textInput = std::make_shared<InputElement>();
        messageHorizontalLayout->addChild(textInput);

        auto send = std::make_shared<ButtonElement>();
        messageHorizontalLayout->addChild(send);
        send->getContainer()->width = {Container::Scale::Label::Pixel, 48.0f};
        send->getContainer()->height = {Container::Scale::Label::Pixel, 48.0f};
        send->fill = "accent-fill";
        send->border = "accent-border";
        send->onPress += [] {
            std::cout << "Button pressed! Message sent!" << std::endl;
        };

        auto icon = std::make_shared<ImageElement>();
        send->addChild(icon);
        icon->image = Texture::load(renderer, "Resources/Images/Send.png");
        icon->getContainer()->x = {Container::Position::Label::Percent, 0.5f};
        icon->getContainer()->y = {Container::Position::Label::Percent, 0.5f};
        icon->getContainer()->xPivot = {Container::Position::Label::Percent, 0.5f};
        icon->getContainer()->yPivot = {Container::Position::Label::Percent, 0.5f};
        icon->getContainer()->width = {Container::Scale::Label::Pixel, 48.0f};
        icon->getContainer()->height = {Container::Scale::Label::Pixel, 48.0f};

        messageSplit->addChild(messageBox);


        auto mainLayer = std::make_shared<UILayer>(renderer);
        mainLayer->addElement(root);

        compositor->addLayer(mainLayer);
    }

    void start() override {
        initRendering();
        buildUI();
    }

    void update(float delta) override {
        //region event handling
        Event event;
        while (Event::getEvent(event)) {
            compositor->pushEvent(event);
            if (auto* close = event.as<WindowCloseEvent>()) {
                quit();
            }
            if (auto* resize = event.as<WindowResizeEvent>()) {
                target = std::make_shared<Texture>(renderer, Texture::Format::R8G8B8A8_SRGB, resize->realWidth,
                                                   resize->realHeight, 1,
                                                   BitFlag<Texture::Flags>({Texture::Flags::RenderTarget}));
            }
        }
        //endregion

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
    }
};

int main() {
    MessageApp app;
    app.run();
    return 0;
}
