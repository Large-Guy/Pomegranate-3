#include "Shader.h"
#include <iostream>
#include "SDL3/SDL_gpu.h"
#include <filesystem>

#include "SDL3_shadercross/SDL_shadercross.h"

void* Shader::getInternal() const {
    return this->shader;
}

Shader::Shader(const std::shared_ptr<Renderer>& renderer, Type type, const std::string& path, uint32_t samplers, uint32_t uniforms, uint32_t storages, uint32_t textures) {
    this->renderer = renderer;
    this->path = path;

    compile(renderer, path, type, samplers, uniforms, storages, textures);   
}

Shader::~Shader() {
    SDL_ReleaseGPUShader(static_cast<SDL_GPUDevice*>(renderer->getInternal()), shader);
}

void Shader::compile(const std::shared_ptr<Renderer>& renderer, const std::string& path, Type stage, uint32_t samplers, uint32_t uniforms, uint32_t storages, uint32_t textures) {
    this->path = path;
    this->stage = stage;
    this->samplers = samplers;
    this->uniforms = uniforms;
    this->storages = storages;
    this->textures = textures;

    SDL_GPUShaderStage shaderStage;
    switch (stage) {
        case Type::Vertex: {
            shaderStage = SDL_GPU_SHADERSTAGE_VERTEX;
            break;
        }
        case Type::Fragment: {
            shaderStage = SDL_GPU_SHADERSTAGE_FRAGMENT;
            break;
        }
        default: {
            throw std::runtime_error("Invalid shader stage");
        }
    }

    const char* entrypoint = nullptr;
    SDL_GPUShaderFormat backendFormat = SDL_GetGPUShaderFormats(static_cast<SDL_GPUDevice*>(renderer->getInternal()));
    SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;


#ifdef __APPLE__

    if (backendFormat & SDL_GPU_SHADERFORMAT_MSL) {
        format = SDL_GPU_SHADERFORMAT_MSL;
        if (shaderStage == SDL_GPU_SHADERSTAGE_VERTEX) {
            entrypoint = "_vertex";
        }
        if (shaderStage == SDL_GPU_SHADERSTAGE_FRAGMENT) {
            entrypoint = "_fragment";
        }
    } else {
        throw std::runtime_error("Could not find GPU shader format");
    }


#else
    if (backendFormat & SDL_GPU_SHADERFORMAT_SPIRV) {
        format = SDL_GPU_SHADERFORMAT_SPIRV;
        if (shaderStage == SDL_GPU_SHADERSTAGE_VERTEX) {
            entrypoint = "_vertex";
        }
        if (shaderStage == SDL_GPU_SHADERSTAGE_FRAGMENT) {
            entrypoint = "_fragment";
        }
    } else {
        throw std::runtime_error("Could not find GPU shader format");
    }
#endif

    size_t codeSize;
    void* code = SDL_LoadFile(path.c_str(), &codeSize);
    
    if (path.ends_with(".hlsl")) {
        std::filesystem::path filePath(path);
        std::filesystem::path shaderPath = filePath.parent_path();
        std::string parentPath = shaderPath.string();
        SDL_ShaderCross_HLSL_Info info = {
            .source = static_cast<const char*>(code),
            .entrypoint = entrypoint,
            .include_dir = parentPath.c_str(),
            .defines = nullptr,
            .shader_stage = stage == Type::Vertex
                                ? SDL_SHADERCROSS_SHADERSTAGE_VERTEX
                                : SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT,
            .props = 0,
        };
        code = SDL_ShaderCross_CompileSPIRVFromHLSL(&info, &codeSize);
    }

    if (!code) {
        throw std::runtime_error("Failed to load shader: " + std::string(SDL_GetError()));
    }


#ifdef __APPLE__
    SDL_ShaderCross_SPIRV_Info transpileInfo = {
        .bytecode = static_cast<const Uint8*>(code),
        .bytecode_size = codeSize,
        .entrypoint = entrypoint,
        .shader_stage = stage == Type::Vertex
                            ? SDL_SHADERCROSS_SHADERSTAGE_VERTEX
                            : SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT,
        .props = 0
    };

    void* transpiledCode = SDL_ShaderCross_TranspileMSLFromSPIRV(&transpileInfo);

    code = transpiledCode;

    if (code == nullptr) {
        throw std::runtime_error("Failed to transpile shader: " + std::string(SDL_GetError()));
    }

    codeSize = SDL_strlen(static_cast<const char*>(code));
#endif


    SDL_GPUShaderCreateInfo shaderInfo = {
        .code_size = codeSize,
        .code = static_cast<const uint8_t*>(code),
        .entrypoint = entrypoint,
        .format = format,
        .stage = shaderStage,
        .num_samplers = samplers,
        .num_storage_textures = textures,
        .num_storage_buffers = storages,
        .num_uniform_buffers = uniforms,
    };

    shader = SDL_CreateGPUShader(static_cast<SDL_GPUDevice*>(renderer->getInternal()), &shaderInfo);

    if (!shader) {
        throw std::runtime_error("Failed to create shader: " + std::string(SDL_GetError()));
    }

    SDL_free(code);
}

Shader::Type Shader::type() {
    return stage;
}
