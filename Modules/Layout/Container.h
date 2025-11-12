#ifndef POMEGRANATE_LAYOUT_H
#define POMEGRANATE_LAYOUT_H

#include <vector>


class Container : public std::enable_shared_from_this<Container> {
public:
    enum class Type {
        Float, //Children can be position freely in the container
        Full, //Children will fill the container
        Vertical, //Children will be slotted vertically
        Horizontal, //Children will be slotted horizontally
    };

    struct Output {
        float x;
        float y;
        float width;
        float height;

        bool contains(float x, float y);
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

    private:
        [[nodiscard]] float real(float relative, float autoSize, float scale = 1.0f) const;

        friend class Container;
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

    private:
        [[nodiscard]] float real(float start, float relative, float scale = 1.0f) const;

        friend class Container;
    };

    Type type;

    Position x;
    Position y;
    Scale width;
    Scale height;

    Scale padding;
    Scale gap;

    bool overflowX;
    bool overflowY;
    bool fillX;
    bool fillY;

    Container(Type type, Position x, Position y, Scale width, Scale height);

    void appendChild(const std::shared_ptr<Container>& container);

    void insertChild(size_t index, const std::shared_ptr<Container>& container);

    void removeChild(size_t index);

    size_t getParentIndex() const;

    size_t getChildCount() const;

    std::shared_ptr<Container> getChild(size_t index) const;

    void computeChildren();

    [[nodiscard]] Output real() const;

private:
    void computeFloat();

    void computeFull();

    void computeVertical();

    void computeHorizontal();

    std::shared_ptr<Container> parent;
    std::vector<std::shared_ptr<Container> > children;

    Output rect{};
};


#endif //POMEGRANATE_LAYOUT_H
