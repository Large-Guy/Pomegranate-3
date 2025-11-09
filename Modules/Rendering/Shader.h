#ifndef POMEGRANATE_SHADER_H
#define POMEGRANATE_SHADER_H
#include <memory>
#include <string>

#include "Renderer.h"


class Shader {
public:
    enum class Type {
        Unknown,
        Vertex,
        Fragment
    };

    void* getInternal() const;
    Shader(const std::shared_ptr<Renderer>& renderer, Type type, const std::string& path, uint32_t samplers, uint32_t uniforms, uint32_t storages, uint32_t textures);

    ~Shader();

    Type type();

protected:
    void compile(const std::shared_ptr<Renderer>& renderer, const std::string& path, Type stage, uint32_t samplers, uint32_t uniforms, uint32_t storages, uint32_t textures);
    
    std::shared_ptr<Renderer> renderer;
    struct SDL_GPUShader* shader = nullptr;
    std::string path;
    Type stage = Type::Unknown;
    uint32_t samplers = 0;
    uint32_t uniforms = 0;
    uint32_t storages = 0;
    uint32_t textures = 0;
};

#endif //POMEGRANATE_SHADER_H
