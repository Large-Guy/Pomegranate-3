#include "Pipeline.h"
#include <SDL3/SDL.h>

#include "RenderPass.h"

Pipeline::Pipeline(const std::shared_ptr<Renderer>& renderer, Texture::Format format) {
    this->renderer = renderer;
    this->vertexShader = nullptr;
    this->fragmentShader = nullptr;
    this->pipeline = nullptr;
    this->format = format;

    this->rasterizer = FillMode::Fill;
    this->cull = CullMode::None;
    this->primitive = PrimitiveType::TriangleList;
}

Pipeline::~Pipeline() {
    if (pipeline != nullptr)
        SDL_ReleaseGPUGraphicsPipeline(static_cast<SDL_GPUDevice*>(renderer->getInternal()), pipeline);
}

void Pipeline::vertex(const std::shared_ptr<Shader>& shader) {
    this->vertexShader = shader;
}

void Pipeline::fragment(const std::shared_ptr<Shader>& shader) {
    this->fragmentShader = shader;
}

void Pipeline::build() {
   
    SDL_GPUTextureFormat form;
    switch (format)
    {
        case Texture::Format::R8G8B8A8_SRGB:
            form = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
            break;
    }

    SDL_GPURasterizerState state{};

    switch (rasterizer) {
        case FillMode::Fill: {
            state.fill_mode = SDL_GPU_FILLMODE_FILL;
            break;
        }
        case FillMode::Wireframe: {
            state.fill_mode = SDL_GPU_FILLMODE_LINE;
            break;
        }
    }

    switch (cull) {
        case CullMode::Back: {
            state.cull_mode = SDL_GPU_CULLMODE_BACK;
            break;
        }
            
        case CullMode::Front: {
            state.cull_mode = SDL_GPU_CULLMODE_FRONT;
            break;
        }
            
        case CullMode::None: {
            state.cull_mode = SDL_GPU_CULLMODE_NONE;
            break;
        }
    }

    SDL_GPUPrimitiveType type;

    switch (primitive) {
        case PrimitiveType::TriangleList:
            type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
            break;
        case PrimitiveType::TriangleStrip:
            type = SDL_GPU_PRIMITIVETYPE_TRIANGLESTRIP;
            break;
        case PrimitiveType::LineList:
            type = SDL_GPU_PRIMITIVETYPE_LINELIST;
            break;
        case PrimitiveType::LineStrip:
            type = SDL_GPU_PRIMITIVETYPE_LINESTRIP;
            break;
        case PrimitiveType::Point:
            type = SDL_GPU_PRIMITIVETYPE_POINTLIST;
            break;
    }

    SDL_GPUGraphicsPipelineCreateInfo pipeline_info;
    pipeline_info = {

        .vertex_shader = static_cast<SDL_GPUShader*>(this->vertexShader->getInternal()),
        .fragment_shader = static_cast<SDL_GPUShader*>(this->fragmentShader->getInternal()),
        .primitive_type = type,
        .rasterizer_state = state,

        .target_info = {
            .color_target_descriptions = (SDL_GPUColorTargetDescription[]){
                    {
                        .format = form,
                        .blend_state = {
                            .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
                            .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                            .color_blend_op = SDL_GPU_BLENDOP_ADD,
                            .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
                            .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                            .alpha_blend_op = SDL_GPU_BLENDOP_ADD,
                            .enable_blend = true,
                        }
                    }
            },
            .num_color_targets = 1,
        },
    };

    this->pipeline = SDL_CreateGPUGraphicsPipeline(static_cast<SDL_GPUDevice*>(renderer->getInternal()), &pipeline_info); 
}

void Pipeline::bind(const std::weak_ptr<CommandQueue>& commandQueue, const std::shared_ptr<DrawPass>& pass) {
    this->queue = commandQueue;
    SDL_BindGPUGraphicsPipeline(static_cast<SDL_GPURenderPass*>(pass->getInternal()), pipeline);
}

void Pipeline::uniformPtr(Shader::Type type, int index, void* info, size_t size) {
    if (queue.expired())
        throw std::runtime_error("Command queue has expired, or Pipeline has not been bound!");
    auto commandQueue = queue.lock();
    switch (type) {
        case Shader::Type::Vertex: {
            SDL_PushGPUVertexUniformData(static_cast<SDL_GPUCommandBuffer*>(commandQueue->getInternal()), index, info, size);
            break;
        }
        case Shader::Type::Fragment: {
            SDL_PushGPUFragmentUniformData(static_cast<SDL_GPUCommandBuffer*>(commandQueue->getInternal()), index, info, size);
            break;
        }
        default:
            throw std::runtime_error("Invalid shader type");
    }
}

