#ifndef POMEGRANATE_FLEXABLE_H
#define POMEGRANATE_FLEXABLE_H
#include "Container.h"


class Flexable : public Container {
public:
    enum class Alignment {
        Leading,
        Center,
        Trailing
    };

    Alignment xAlignment;
    Alignment yAlignment;
    bool xOverflow;
    bool yOverflow;
    bool xFill;
    bool yFill;
};


#endif //POMEGRANATE_FLEXABLE_H
