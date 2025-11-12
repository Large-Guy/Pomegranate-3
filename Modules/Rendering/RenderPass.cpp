#include "RenderPass.h"

#include <iostream>
#include <ostream>

RenderPass::RenderPass(const std::shared_ptr<Renderer>& renderer) {
    this->renderer = renderer;
    this->renderPass = nullptr;
}


void* RenderPass::getInternal() const {
    return this->renderPass;
}

