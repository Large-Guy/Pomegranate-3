#ifndef POMEGRANATE_VERTICAL_H
#define POMEGRANATE_VERTICAL_H
#include "Flexable.h"


class Vertical final : public Flexable {
public:
    void computeRect(float scale) override;
};


#endif //POMEGRANATE_VERTICAL_H
