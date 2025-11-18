#ifndef POMEGRANATE_ROOT_H
#define POMEGRANATE_ROOT_H
#include "UI/UIElement.h"


class Root : public UIElement {
public:
    Root();

    void render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                const std::weak_ptr<CommandBuffer>& commandBuffer,
                const std::shared_ptr<DrawPass>& drawPass, const std::shared_ptr<Texture>& background) override;
};


#endif //POMEGRANATE_ROOT_H
