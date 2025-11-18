#ifndef POMEGRANATE_NODE_H
#define POMEGRANATE_NODE_H
#include <memory>
#include <vector>


template<typename T>
class Node : public std::enable_shared_from_this<T> {
public:
    using iterator = std::vector<std::shared_ptr<T> >::iterator;
    using const_iterator = std::vector<std::shared_ptr<T> >::const_iterator;

    Node() = default;

    virtual ~Node() = default;

    void addChild(const std::shared_ptr<T>& node) {
        this->children.push_back(node);
        std::dynamic_pointer_cast<Node>(node)->parent = this->shared_from_this();
        node->onAdded(std::dynamic_pointer_cast<T>(this->shared_from_this()));
        this->onChildAdded(node);
    }

    void removeChild(const std::shared_ptr<T>& node) {
        this->onChildRemoved(node);
        node->onRemoved(std::dynamic_pointer_cast<T>(this->shared_from_this()));
        std::erase(this->children, node);
        std::dynamic_pointer_cast<Node>(node)->parent.reset();
    }

    void removeChildAt(size_t index) {
        this->onChildRemoved(this->children[index]);
        children[index]->onRemoved(this->shared_from_this());
        std::dynamic_pointer_cast<Node>(children[index])->parent.reset();
        children.erase(this->children.begin() + index);
    }

    size_t getChildCount() const {
        return this->children.size();
    }

    std::shared_ptr<T> getChild(size_t index) {
        return this->children[index];
    }

    template<typename S>
    std::shared_ptr<S> getChild(size_t index) {
        return std::dynamic_pointer_cast<S>(this->getChild(index));
    }

    std::shared_ptr<T> getParent() {
        return this->parent.lock();
    }

    template<typename S>
    std::shared_ptr<S> getParent() {
        return std::dynamic_pointer_cast<S>(this->getParent());
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
        return std::dynamic_pointer_cast<S>(this->getRoot());
    }

    void destroy() {
        if (this->parent.expired())
            return;
        this->parent.lock()->removeChild(this->shared_from_this());
    }

    iterator begin() {
        return this->children.begin();
    }

    iterator end() {
        return this->children.end();
    }

    const_iterator begin() const {
        return this->children.begin();
    }

    const_iterator end() const {
        return this->children.end();
    }

    const_iterator cbegin() const {
        return this->children.cbegin();
    }

    const_iterator cend() const {
        return this->children.cend();
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
