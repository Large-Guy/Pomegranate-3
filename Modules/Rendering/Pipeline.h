#ifndef POMEGRANATE_PIPELINE_H
#define POMEGRANATE_PIPELINE_H
#include <memory>

#include "DrawPass.h"
#include "Shader.h"
#include "Texture.h"


class Pipeline {
public:
    enum class FillMode {
        Fill,
        Wireframe,
    };
    enum class CullMode {
        Front,
        Back,
        None
    };
    enum class PrimitiveType {
        TriangleList,
        TriangleStrip,
        LineList,
        LineStrip,
        Point,
    };
    
    Pipeline(const std::shared_ptr<Renderer>& renderer, Texture::Format format);
    ~Pipeline();

    void vertex(const std::shared_ptr<Shader>& shader);
    void fragment(const std::shared_ptr<Shader>& shader);
    void build();

    void bind(const std::weak_ptr<CommandQueue>& commandQueue, const std::shared_ptr<DrawPass>& pass);

    void uniformPtr(Shader::Type type, int index, void* info, size_t size);

    template <typename T>
    void uniform(Shader::Type type, const T& info) {
        uniformPtr(type, index, &info, sizeof(T));
    }
private:
    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<Shader> vertexShader;
    std::shared_ptr<Shader> fragmentShader;
    Texture::Format format;
    FillMode rasterizer;
    CullMode cull;
    PrimitiveType primitive;
    struct SDL_GPUGraphicsPipeline* pipeline;
    std::weak_ptr<CommandQueue> queue;
};


#endif //POMEGRANATE_PIPELINE_H