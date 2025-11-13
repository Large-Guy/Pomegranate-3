#ifndef POMEGRANATE_LAYOUT_H
#define POMEGRANATE_LAYOUT_H

#include <vector>


class Container : public std::enable_shared_from_this<Container> {
public:
    virtual ~Container() = default;

    using iterator = std::vector<std::shared_ptr<Container> >::iterator;
    using const_iterator = std::vector<std::shared_ptr<Container> >::const_iterator;

    struct Output {
        float x;
        float y;
        float width;
        float height;

        bool contains(float x, float y) const;
    };

    struct Scale {
        enum class Label {
            Auto, //This container will be scaled automatically
            Percent, //This container will be scaled based on a percentage
            Pixel, //This container will be scaled based on an exact logical pixels
            Physical, //This container will be scaled based on exact physical pixels
        };

        Label label;
        float value;

        Scale();

        Scale(Label label, float value);

        Scale& operator=(float value);

        Scale& operator=(Label label);

        [[nodiscard]] float real(float relative, float autoSize, float scale = 1.0f) const;
    };

    struct Position {
        enum class Label {
            Auto, //This container will be position automatically
            Percent, //This container will be offset based on a percentage of it's parent
            Pixel, //This container will be offset based on logical pixels
            Physical, //This container will be offset based on physical pixels
        };

        Label label;
        float value;

        Position();

        Position(Label label, float value);

        Position& operator=(float value);

        Position& operator=(Label label);

        [[nodiscard]] float real(float start, float relative, float scale = 1.0f) const;
    };

    Position x = {Position::Label::Auto, 0.0f};
    Position y = {Position::Label::Auto, 0.0f};
    Scale width = {Scale::Label::Auto, 1.0f};
    Scale height = {Scale::Label::Auto, 1.0f};

    Scale padding = {Scale::Label::Auto, 0.0f};
    Scale gap = {Scale::Label::Auto, 0.0f};

    Output rect{};

    void appendChild(const std::shared_ptr<Container>& container);

    void insertChild(size_t index, const std::shared_ptr<Container>& container);

    void removeChild(size_t index);

    size_t getParentIndex() const;

    size_t getChildCount() const;

    std::shared_ptr<Container> getChild(size_t index) const;

    template<typename T>
    std::shared_ptr<T> getChild(size_t index) const {
        return std::dynamic_pointer_cast<T>(getChild(index));
    }

    virtual void compute();

    void computeChildren();

    [[nodiscard]] Output real() const;

    iterator begin();

    iterator end();

    const_iterator begin() const;

    const_iterator end() const;

    const_iterator cbegin() const;

    const_iterator cend() const;

private:
    std::shared_ptr<Container> parent;
    std::vector<std::shared_ptr<Container> > children;
};


#endif //POMEGRANATE_LAYOUT_H
