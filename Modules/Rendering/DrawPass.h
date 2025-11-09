#ifndef POMEGRANATE_DRAWPASS_H
#define POMEGRANATE_DRAWPASS_H
#include "RenderPass.h"

class DrawPass final : public RenderPass {
public:
    explicit DrawPass(const std::shared_ptr<Renderer>& renderer);
    ~DrawPass() override;

    void clear(Color color);
    void viewport(Viewport viewport);
    void texture(std::shared_ptr<Texture> texture);
    void begin(const std::weak_ptr<CommandQueue>& commandQueue) override;
    void end() override;
private:
    enum Elements {
        ElementClear,
        ElementViewport,
        ElementTexture,
    };
    
    Color clearColor;
    Viewport vp{};
    std::shared_ptr<Texture> outputTexture;
    BitFlag<Elements> elements;
};



#endif //POMEGRANATE_DRAWPASS_H