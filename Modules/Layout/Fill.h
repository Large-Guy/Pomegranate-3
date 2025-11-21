#ifndef POMEGRANATE_FILL_H
#define POMEGRANATE_FILL_H
#include "Flexable.h"


class Fill : public Flexable {
public:
    void computeRect(float scale) override;

    static std::shared_ptr<Fill> make();

protected:
    Fill() = default;
};


#endif //POMEGRANATE_FILL_H
