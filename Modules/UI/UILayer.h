#ifndef POMEGRANATE_UILAYER_H
#define POMEGRANATE_UILAYER_H
#include <memory>

#include "UIElement.h"
#include "Events/Event.h"
#include "Rendering/Texture.h"


class UILayer {
public:
    UILayer(const std::shared_ptr<Renderer>& compositor);

    std::shared_ptr<Texture> render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                                    const std::weak_ptr<CommandBuffer>& commandBuffer,
                                    const std::shared_ptr<Texture>& background);

    void pushEvent(Event& event);

    void addElement(const std::shared_ptr<UIElement>& element);

    void removeElement(int index);

private:
    static void drawElements(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                             const std::weak_ptr<CommandBuffer>& commandBuffer,
                             const std::shared_ptr<DrawPass>& drawPass,
                             const std::shared_ptr<Texture>& background, const std::shared_ptr<UIElement>& element);

    std::shared_ptr<Renderer> renderer;
    std::vector<std::shared_ptr<UIElement> > elements;
    std::shared_ptr<Texture> texture;
};


#endif //POMEGRANATE_UILAYER_H
