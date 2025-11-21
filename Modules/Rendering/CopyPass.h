#ifndef POMEGRANATE_COPYPASS_H
#define POMEGRANATE_COPYPASS_H
#include "RenderPass.h"

class CopyPass final : public RenderPass {
public:
    void begin(const std::weak_ptr<CommandBuffer>& commandQueue) override;

    void end() override;

    static std::shared_ptr<CopyPass> make(const std::shared_ptr<Renderer>& renderer);

private:
    CopyPass(const std::shared_ptr<Renderer>& renderer);
};

#endif //POMEGRANATE_COPYPASS_H
