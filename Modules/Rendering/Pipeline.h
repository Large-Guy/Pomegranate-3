#ifndef POMEGRANATE_PIPELINE_H
#define POMEGRANATE_PIPELINE_H
#include <memory>

#include "DrawPass.h"
#include "Sampler.h"
#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"


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

    void vertex(const std::shared_ptr<Shader>& shader, const VertexDescription& description);

    void fragment(const std::shared_ptr<Shader>& shader);

    void primitive(PrimitiveType type);

    void fill(Pipeline::FillMode fillMode);

    void build();

    void bind(const std::weak_ptr<CommandQueue>& commandQueue, const std::shared_ptr<DrawPass>& pass);

    void uniformPtr(Shader::Type type, int index, void* info, size_t size) const;

    template<typename T>
    void uniform(Shader::Type type, int index, T& info) {
        uniformPtr(type, index, static_cast<void*>(&info), sizeof(T));
    }

    void texture(int index, const std::shared_ptr<Texture>& texture, const std::shared_ptr<Sampler>& sampler);

private:
    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<Shader> vertexShader;
    VertexDescription vertexDescription;
    std::shared_ptr<Shader> fragmentShader;
    Texture::Format format;
    FillMode rasterizer;
    CullMode cull;
    PrimitiveType primitiveType;
    struct SDL_GPUGraphicsPipeline* pipeline;
    std::weak_ptr<CommandQueue> queue;
    std::weak_ptr<DrawPass> pass;
    std::vector<std::shared_ptr<Texture> > textures;
    std::vector<std::shared_ptr<Sampler> > samplers;
};


#endif //POMEGRANATE_PIPELINE_H
