#ifndef POMEGRANATE_HORIZONTAL_H
#define POMEGRANATE_HORIZONTAL_H
#include "Flexable.h"


class Horizontal : public Flexable {
public:
    void computeRect(float scale) override;

    static std::shared_ptr<Horizontal> make();

protected:
    Horizontal() = default;
};


#endif //POMEGRANATE_HORIZONTAL_H
