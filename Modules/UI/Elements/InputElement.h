#ifndef POMEGRANATE_TEXTINPUT_H
#define POMEGRANATE_TEXTINPUT_H
#include "PanelElement.h"
#include "TextElement.h"


class InputElement : public PanelElement {
public:
    InputElement();

    void render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                const std::weak_ptr<CommandBuffer>& commandBuffer, const std::shared_ptr<DrawPass>& drawPass,
                const std::shared_ptr<Texture>& background) override;

protected:
    std::shared_ptr<Container> textContainer;
};


#endif //POMEGRANATE_TEXTINPUT_H
