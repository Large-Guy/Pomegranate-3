#ifndef POMEGRANATE_UIELEMENT_H
#define POMEGRANATE_UIELEMENT_H
#include <memory>

#include "Theme.h"
#include "Layout/Container.h"
#include "Nodes/Node.h"
#include "Rendering/CommandBuffer.h"
#include "Rendering/DrawPass.h"


class UIElement : public Node<UIElement> {
public:
    using iterator = typename std::vector<std::shared_ptr<UIElement> >::iterator;
    using const_iterator = typename std::vector<std::shared_ptr<UIElement> >::const_iterator;

    explicit UIElement(const std::shared_ptr<Container>& container);

    virtual ~UIElement();

    virtual void onAddedToLayer(const std::shared_ptr<Renderer>& renderer);

    virtual void onRemovedFromLayer();

    virtual void render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                        const std::weak_ptr<CommandBuffer>& commandBuffer,
                        const std::shared_ptr<DrawPass>& drawPass,
                        const std::shared_ptr<Texture>& background);

    std::shared_ptr<Container> getContainer();

    template<typename T>
    std::shared_ptr<T> getContainer() {
        return std::dynamic_pointer_cast<T>(getContainer());
    }

protected:
    void onChildAdded(const std::shared_ptr<UIElement>& child) override;

    void onChildRemoved(const std::shared_ptr<UIElement>& child) override;

private:
    std::shared_ptr<Container> container;
};


#endif //POMEGRANATE_UIELEMENT_H
