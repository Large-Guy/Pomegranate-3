#ifndef POMEGRANATE_TRANSFERBUFFER_H
#define POMEGRANATE_TRANSFERBUFFER_H
#include "Renderer.h"


class TransferBuffer {
public:
    enum class Type {
        Upload,
        Download
    };

    ~TransferBuffer();

    void* mapPtr() const;

    void unmap() const;

    template<typename T>
    T* map() {
        return static_cast<T*>(mapPtr());
    }

    size_t size() const;

    void* getInternal();

    static std::shared_ptr<TransferBuffer> make(const std::shared_ptr<Renderer>& renderer, Type type, size_t size);

private:
    TransferBuffer(std::shared_ptr<Renderer> renderer, Type type, size_t size);

    std::shared_ptr<Renderer> renderer;
    size_t bufferSize;
    struct SDL_GPUTransferBuffer* buffer;
};


#endif //POMEGRANATE_TRANSFERBUFFER_H
