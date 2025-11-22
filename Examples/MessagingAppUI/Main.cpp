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
        window = Window::make();
        window->setTitle("Messaging App");
        window->setSize(1024, 720);

        window->open();


        window->setFlag(Window::Resizable, true);
        window->setFlag(Window::Transparent, true);
        window->setFlag(Window::Blur, true);
        window->setFlag(Window::Full, true);
        window->setFlag(Window::HighDPI, true);

        renderer = Renderer::make();
        target = Texture::make(renderer, Texture::Format::R8G8B8A8_SRGB, 1024, 720, 1,
                               BitFlag<Texture::Flags>({Texture::Flags::RenderTarget}));

        display = Display::make(renderer, window);
    }

    void buildUI() {
        theme = Theme::dark();
        compositor = UICompositor::make(renderer);

        auto root = RootElement::make();

        //Split the screen vertically
        auto split = HorizontalLayoutElement::make();
        root->addChild(split);
        split->getContainer<Flexable>()->xFill = true;
        split->getContainer<Flexable>()->xOverflow = false;
        split->getContainer<Flexable>()->gap = 0;

        //People
        auto contacts = PanelElement::make();
        split->addChild(contacts);
        contacts->getContainer()->width = {Container::Scale::Label::Pixel, 256.0f};

        //Your messages with the selected chatter
        auto messages = PanelElement::make();
        split->addChild(messages);
        messages->getContainer()->width = {Container::Scale::Label::Auto, 0.0f};
        messages->fill = "secondary-fill";
        messages->border = "secondary-border";

        auto messageSplit = VerticalLayoutElement::make();
        messageSplit->getContainer()->gap = {Container::Scale::Label::Pixel, 0.0f};
        messageSplit->getContainer<Vertical>()->yOverflow = false;
        messages->addChild(messageSplit);
        auto messageSpacer = FullLayoutElement::make();
        messageSplit->addChild(messageSpacer);

        auto messageContents = VerticalLayoutElement::make();
        messageSpacer->addChild(messageContents);

        //Add some text
        auto text = TextElement::make();
        messageContents->addChild(text);
        text->text = "Hello, World!";

        auto messageBox = FullLayoutElement::make();
        messageBox->getContainer()->height = {Container::Scale::Label::Pixel, 64.0f};

        auto messageHorizontalLayout = HorizontalLayoutElement::make();
        messageBox->addChild(messageHorizontalLayout);

        auto textInput = InputElement::make();
        messageHorizontalLayout->addChild(textInput);

        auto send = ButtonElement::make();
        messageHorizontalLayout->addChild(send);
        send->getContainer()->width = {Container::Scale::Label::Pixel, 48.0f};
        send->getContainer()->height = {Container::Scale::Label::Pixel, 48.0f};
        send->fill = "accent-fill";
        send->border = "accent-border";
        send->onPress += [] {
            std::cout << "Button pressed! Message sent!" << std::endl;
        };

        auto icon = ImageElement::make();
        send->addChild(icon);
        icon->image = Texture::load(renderer, "Resources/Images/Send.png");
        icon->getContainer()->x = {Container::Position::Label::Percent, 0.5f};
        icon->getContainer()->y = {Container::Position::Label::Percent, 0.5f};
        icon->getContainer()->xPivot = {Container::Position::Label::Percent, 0.5f};
        icon->getContainer()->yPivot = {Container::Position::Label::Percent, 0.5f};
        icon->getContainer()->width = {Container::Scale::Label::Pixel, 48.0f};
        icon->getContainer()->height = {Container::Scale::Label::Pixel, 48.0f};

        messageSplit->addChild(messageBox);


        auto mainLayer = UILayer::make(renderer);
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
                target = Texture::make(renderer, Texture::Format::R8G8B8A8_SRGB, resize->realWidth,
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
