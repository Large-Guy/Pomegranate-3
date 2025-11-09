#ifndef POMEGRANATE_TRANSFERBUFFER_H
#define POMEGRANATE_TRANSFERBUFFER_H
#include "Renderer.h"


class TransferBuffer {
public:
    enum class Type {
        Upload,
        Download
    };
    TransferBuffer(std::shared_ptr<Renderer> renderer, Type type, size_t size);
    ~TransferBuffer();

    void* map() const;
    void unmap() const;

    size_t size() const;

    void* getInternal();
private:
    std::shared_ptr<Renderer> renderer;
    size_t bufferSize;
    struct SDL_GPUTransferBuffer* buffer;
};


#endif //POMEGRANATE_TRANSFERBUFFER_H