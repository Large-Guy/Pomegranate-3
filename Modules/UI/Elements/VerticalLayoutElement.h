#ifndef POMEGRANATE_VERTICALLAYOUT_H
#define POMEGRANATE_VERTICALLAYOUT_H
#include "UI/UIElement.h"


class VerticalLayoutElement : public UIElement {
public:
    static std::shared_ptr<VerticalLayoutElement> make();

protected:
    VerticalLayoutElement();
};


#endif //POMEGRANATE_VERTICALLAYOUT_H
