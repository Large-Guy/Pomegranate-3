#ifndef POMEGRANATE_BUFFER_H
#define POMEGRANATE_BUFFER_H
#include "Renderer.h"


class Buffer {
public:
    enum class Type {
        Vertex,
        Index,
    };
    Buffer(const std::shared_ptr<Renderer>& renderer, Type usage, size_t size);
    ~Buffer();

    Type type();
private:
    struct SDL_GPUBuffer* buffer;
    struct std::shared_ptr<Renderer> renderer;
    size_t size;
    Type usage;
};


#endif //POMEGRANATE_BUFFER_H