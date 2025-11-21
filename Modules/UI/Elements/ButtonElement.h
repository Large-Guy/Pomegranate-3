#ifndef POMEGRANATE_BUTTONELEMENT_H
#define POMEGRANATE_BUTTONELEMENT_H
#include "PanelElement.h"
#include "Core/Signal.h"
#include "UI/UIElement.h"


class ButtonElement : public PanelElement {
public:
    Signal<> onPress;
    Signal<> onRelease;

    void onPressed(MouseEvent::Button button) override;

    void onReleased(MouseEvent::Button button) override;

    void render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                const std::weak_ptr<CommandBuffer>& commandBuffer, const std::shared_ptr<DrawPass>& drawPass,
                const std::shared_ptr<Texture>& background) override;

    static std::shared_ptr<ButtonElement> make();

protected:
    ButtonElement();
};


#endif //POMEGRANATE_BUTTONELEMENT_H
