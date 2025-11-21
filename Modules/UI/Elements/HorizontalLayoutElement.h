#ifndef POMEGRANATE_HORIZONTALLAYOUT_H
#define POMEGRANATE_HORIZONTALLAYOUT_H
#include "UI/UIElement.h"

class HorizontalLayoutElement : public UIElement {
public:
    static std::shared_ptr<HorizontalLayoutElement> make();

protected:
    HorizontalLayoutElement();
};


#endif //POMEGRANATE_HORIZONTALLAYOUT_H
