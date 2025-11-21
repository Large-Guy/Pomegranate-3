#ifndef POMEGRANATE_SPACER_H
#define POMEGRANATE_SPACER_H
#include "UI/UIElement.h"


class SpacerElement : public UIElement {
public:
    static std::shared_ptr<SpacerElement> make();

protected:
    SpacerElement();
};


#endif //POMEGRANATE_SPACER_H
