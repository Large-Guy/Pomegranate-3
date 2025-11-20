#ifndef POMEGRANATE_UIELEMENT_H
#define POMEGRANATE_UIELEMENT_H
#include <memory>

#include "Theme.h"
#include "Events/Event.h"
#include "Layout/Container.h"
#include "Nodes/Node.h"
#include "Rendering/CommandBuffer.h"
#include "Rendering/DrawPass.h"


class UIElement : public TreeNode<UIElement> {
public:
    static std::weak_ptr<UIElement> focused;

    enum class EventResult {
        Propagate,
        Consumed,
    };

    using iterator = typename std::vector<std::shared_ptr<UIElement> >::iterator;
    using const_iterator = typename std::vector<std::shared_ptr<UIElement> >::const_iterator;

    explicit UIElement(const std::shared_ptr<Container>& container);

    virtual ~UIElement();

    std::shared_ptr<Container> getContainer();

    template<typename T>
    std::shared_ptr<T> getContainer() {
        return std::dynamic_pointer_cast<T>(getContainer());
    }

    void standardEventHandler(Event& event);

    virtual EventResult onEvent(Event& event);

    virtual void onAddedToLayer(const std::shared_ptr<Renderer>& renderer);

    virtual void onRemovedFromLayer();

    virtual void onCursorEnter();

    virtual void onCurserExit();

    virtual void onPressed(MouseEvent::Button button);

    virtual void onReleased(MouseEvent::Button button);

    virtual void render(Viewport screen, float scale, const std::shared_ptr<Theme>& theme,
                        const std::weak_ptr<CommandBuffer>& commandBuffer,
                        const std::shared_ptr<DrawPass>& drawPass,
                        const std::shared_ptr<Texture>& background);

    bool isHovered() const;

protected:
    void onChildAdded(const std::shared_ptr<UIElement>& child) override;

    void onChildRemoved(const std::shared_ptr<UIElement>& child) override;

private:
    std::shared_ptr<Container> container;

    bool hovering = false;
    bool pressed = false;
};


#endif //POMEGRANATE_UIELEMENT_H
