#ifndef POMEGRANATE_UIELEMENT_H
#define POMEGRANATE_UIELEMENT_H
#include <memory>

#include "Layout/Container.h"
#include "Rendering/CommandBuffer.h"
#include "Rendering/DrawPass.h"


class UIElement : public std::enable_shared_from_this<UIElement> {
public:
    using iterator = typename std::vector<std::shared_ptr<UIElement> >::iterator;
    using const_iterator = typename std::vector<std::shared_ptr<UIElement> >::const_iterator;

    UIElement(const std::shared_ptr<Container>& container);

    virtual ~UIElement();

    virtual void onAddedToLayer(const std::shared_ptr<Renderer>& renderer);

    virtual void onRemovedFromLayer();

    virtual void render(Viewport screen, const std::weak_ptr<CommandBuffer>& commandBuffer,
                        const std::shared_ptr<DrawPass>& drawPass,
                        const std::shared_ptr<Texture>& background);

    std::shared_ptr<Container> getContainer();

    void addChild(const std::shared_ptr<UIElement>& child);

    void removeChild(int index);

    std::shared_ptr<UIElement> getChild(int index);

    std::weak_ptr<UIElement> getParent();

    iterator begin();

    iterator end();

    const_iterator begin() const;

    const_iterator end() const;

    const_iterator cbegin() const;

    const_iterator cend() const;

protected:
    virtual void addChildImpl(const std::shared_ptr<UIElement>& child);

    virtual void removeChildImpl(int index);

private:
    std::shared_ptr<Container> container;
    std::weak_ptr<UIElement> parent;
    std::vector<std::shared_ptr<UIElement> > children;
};


#endif //POMEGRANATE_UIELEMENT_H
