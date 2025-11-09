#ifndef POMEGRANATE_SURFACE_H
#define POMEGRANATE_SURFACE_H
#include <memory>

#include "Texture.h"
#include "Platform/Window.h"

class Display {
public:
    explicit Display(const std::shared_ptr<Renderer>& renderer, const std::shared_ptr<Window>& window);
    ~Display();

    void target(const std::weak_ptr<CommandQueue>& queue, const std::shared_ptr<Texture>& renderTexture);
    void present() const;
private:
    std::shared_ptr<Window> window;
    std::shared_ptr<Renderer> renderer;
    struct SDL_Renderer* sdlRenderer;
    struct SDL_Texture* texture;
    int width, height;
    std::shared_ptr<TransferBuffer> transferBuffer;
};


#endif //POMEGRANATE_SURFACE_H