#ifndef POMEGRANATE_COPYPASS_H
#define POMEGRANATE_COPYPASS_H
#include "RenderPass.h"

class CopyPass final : public RenderPass {
public:
    CopyPass(const std::shared_ptr<Renderer>& renderer);
    
    void begin(const std::weak_ptr<CommandBuffer>& commandQueue) override;

    void end() override;
};

#endif //POMEGRANATE_COPYPASS_H