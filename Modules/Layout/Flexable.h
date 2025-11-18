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

    Flexable();

    Alignment xAlignment = Alignment::Leading;
    Alignment yAlignment = Alignment::Leading;
    bool xOverflow = false;
    bool yOverflow = false;
    bool xFill = false;
    bool yFill = false;
};


#endif //POMEGRANATE_FLEXABLE_H
