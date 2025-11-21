#ifndef POMEGRANATE_SAMPLER_H
#define POMEGRANATE_SAMPLER_H
#include <memory>

#include "Renderer.h"


class Sampler {
public:
    enum class Filter {
        Nearest,
        Linear,
    };

    enum class Wrap {
        Clamp,
        Repeat,
        Mirrored
    };

    void* getInternal();

    static std::shared_ptr<Sampler> make(const std::shared_ptr<Renderer>& renderer, Filter filter = Filter::Linear,
                                         Wrap wrap = Wrap::Repeat);

private:
    Sampler(const std::shared_ptr<Renderer>& renderer, Filter filter, Wrap wrap);

    std::shared_ptr<Renderer> renderer;
    struct SDL_GPUSampler* sampler;
    Filter filter;
    Wrap wrap;
};


#endif //POMEGRANATE_SAMPLER_H
