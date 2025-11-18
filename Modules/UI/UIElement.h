#ifndef POMEGRANATE_UIELEMENT_H
#define POMEGRANATE_UIELEMENT_H
#include <memory>

#include "Theme.h"
#include "Layout/Container.h"
#include "Rendering/CommandBuffer.h"
#include "Rendering/DrawPass.h"


class UIElement : public std::enable_shared_from_this<UIElement> {
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

    void addChild(const std::shared_ptr<UIElement>& child);

    void removeChild(int index);

    std::shared_ptr<UIElement> getChild(int index);

    std::weak_ptr<UIElement> getParent() const;

    std::weak_ptr<UIElement> getRoot();

    iterator begin();

    iterator end();

    const_iterator begin() const;

    const_iterator end() const;

    const_iterator cbegin() const;

    const_iterator cend() const;

protected:
    virtual void onChildAdded(const std::shared_ptr<UIElement>& child);

    virtual void onChildRemoved(int index);

private:
    std::shared_ptr<Container> container;
    std::weak_ptr<UIElement> parent;
    std::vector<std::shared_ptr<UIElement> > children;
};


#endif //POMEGRANATE_UIELEMENT_H
