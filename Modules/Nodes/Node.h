#ifndef POMEGRANATE_NODE_H
#define POMEGRANATE_NODE_H
#include <memory>
#include <vector>


template<typename T>
class Node : public std::enable_shared_from_this<Node<T> > {
public:
    using iterator = std::vector<std::shared_ptr<T> >::iterator;
    using const_iterator = std::vector<std::shared_ptr<T> >::const_iterator;

    Node() = default;

    virtual ~Node() = default;

    void addChild(const std::shared_ptr<T>& node) {
        children.push_back(node);
        node->onAdded(std::dynamic_pointer_cast<T>(this->shared_from_this()));
        onChildAdded(node);
    }

    void removeChild(const std::shared_ptr<T>& node) {
        onChildRemoved(node);
        node->onRemoved(std::dynamic_pointer_cast<T>(this->shared_from_this()));
        std::erase(children, node);
    }

    void removeChildAt(size_t index) {
        onChildRemoved(children[index]);
        children[index]->onRemoved(this->shared_from_this());
        children.erase(children.begin() + index);
    }

    size_t getChildCount() const {
        return children.size();
    }

    std::shared_ptr<T> getChild(size_t index) {
        return children[index];
    }

    template<typename S>
    std::shared_ptr<S> getChild(size_t index) {
        return std::dynamic_pointer_cast<S>(getChild(index));
    }

    std::shared_ptr<T> getParent() {
        return parent.lock();
    }

    template<typename S>
    std::shared_ptr<S> getParent() {
        return std::dynamic_pointer_cast<S>(getParent());
    }

    std::shared_ptr<T> getRoot() {
        auto root = this->weak_from_this();
        while (auto current = root.lock()) {
            if (current->getParent()) {
                root = current->getParent();
            }
        }
        return root.lock();
    }

    template<typename S>
    std::shared_ptr<S> getRoot() {
        return std::dynamic_pointer_cast<S>(getRoot());
    }

    void destroy() {
        if (parent.expired())
            return;
        parent.lock()->removeChild(this->shared_from_this());
    }

    iterator begin() {
        return children.begin();
    }

    iterator end() {
        return children.end();
    }

    const_iterator begin() const {
        return children.begin();
    }

    const_iterator end() const {
        return children.end();
    }

    const_iterator cbegin() const {
        return children.cbegin();
    }

    const_iterator cend() const {
        return children.cend();
    }

protected:
    virtual void onAdded(const std::shared_ptr<T>& node) {
    }

    virtual void onRemoved(const std::shared_ptr<T>& node) {
    }

    virtual void onChildAdded(const std::shared_ptr<T>& node) {
    }

    virtual void onChildRemoved(const std::shared_ptr<T>& node) {
    }

private:
    std::weak_ptr<T> parent;
    std::vector<std::shared_ptr<T> > children = {};
};


#endif //POMEGRANATE_NODE_H
