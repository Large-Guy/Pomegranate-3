#ifndef POMEGRANATE_FULL_H
#define POMEGRANATE_FULL_H
#include "UI/UIElement.h"


class FullLayoutElement : public UIElement {
public:
    static std::shared_ptr<FullLayoutElement> make();

protected:
    FullLayoutElement();
};


#endif //POMEGRANATE_FULL_H
