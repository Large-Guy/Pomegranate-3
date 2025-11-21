#ifndef POMEGRANATE_VERTICAL_H
#define POMEGRANATE_VERTICAL_H
#include "Flexable.h"


class Vertical final : public Flexable {
public:
    void computeRect(float scale) override;

    static std::shared_ptr<Vertical> make();

protected:
    Vertical() = default;
};


#endif //POMEGRANATE_VERTICAL_H
