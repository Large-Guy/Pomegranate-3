#ifndef POMEGRANATE_LAYOUT_H
#define POMEGRANATE_LAYOUT_H

#include <vector>

#include "Core/Reflective.h"
#include "Nodes/Node.h"


class Container : public Node<Container>, public Reflective {
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

        [[nodiscard]] float real(float relative, float autoSize, float scale, float percentScale) const;
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

        [[nodiscard]] float real(float start, float relative, float scale) const;
    };

    Position x = {Position::Label::Auto, 0.0f};
    Position y = {Position::Label::Auto, 0.0f};
    Scale width = {Scale::Label::Auto, 0.0f};
    Scale height = {Scale::Label::Auto, 0.0f};

    Scale padding = {Scale::Label::Auto, 0.0f};
    Scale gap = {Scale::Label::Auto, 0.0f};

    Output rect{};

    Position xPivot;
    Position yPivot;

    Position xOffset = {Position::Label::Auto, 0.0f};
    Position yOffset = {Position::Label::Auto, 0.0f};

    Container();

    void compute(float scale = 1.0f);

    [[nodiscard]] Output real() const;

protected:
    virtual void computeRect(float scale);
};


#endif //POMEGRANATE_LAYOUT_H
