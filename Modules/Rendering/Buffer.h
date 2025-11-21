#ifndef POMEGRANATE_BUFFER_H
#define POMEGRANATE_BUFFER_H
#include "DrawPass.h"
#include "Renderer.h"
#include "RenderPass.h"


class Buffer {
public:
    enum class Type {
        Vertex,
        Index,
    };

    ~Buffer();

    void upload(const std::shared_ptr<CopyPass>& pass, const std::shared_ptr<TransferBuffer>& data);

    void bind(const std::shared_ptr<DrawPass>& pass) const;

    void unbind(const std::shared_ptr<DrawPass>& pass) const;

    Type type();

    static std::shared_ptr<Buffer> make(const std::shared_ptr<Renderer>& renderer, Type usage, size_t size);

private:
    Buffer(const std::shared_ptr<Renderer>& renderer, Type usage, size_t size);


    struct SDL_GPUBuffer* buffer;
    struct std::shared_ptr<Renderer> renderer;
    size_t size;
    Type usage;
};


#endif //POMEGRANATE_BUFFER_H
