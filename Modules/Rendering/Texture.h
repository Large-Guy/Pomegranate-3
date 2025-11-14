#ifndef POMEGRANATE_TEXTURE_H
#define POMEGRANATE_TEXTURE_H
#include "Renderer.h"
#include <memory>

#include "TransferBuffer.h"
#include "Core/BitFlag.h"
#include "Platform/Window.h"

class Texture {
public:
    enum class Format {
        R8G8B8A8_SRGB,
    };

    enum Flags {
        RenderTarget,
    };

    Texture(const std::shared_ptr<Renderer>& renderer, Format format, int width, int height, int layers,
            BitFlag<Flags> flags);

    ~Texture();

    void download(const std::shared_ptr<struct CopyPass>& renderPass, const std::shared_ptr<TransferBuffer>& output);

    int width() const;

    int height() const;

    void* getInternal() const;

private:
    std::shared_ptr<Renderer> renderer;
    struct SDL_GPUTexture* texture;
    Format format;
    BitFlag<Flags> flags;
    int textureWidth, textureHeight;

    std::shared_ptr<Window> window;
};


#endif //POMEGRANATE_TEXTURE_H
