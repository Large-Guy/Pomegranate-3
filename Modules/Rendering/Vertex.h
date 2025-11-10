#ifndef POMEGRANATE_VERTEX_H
#define POMEGRANATE_VERTEX_H
#include <vector>

struct VertexDescription {
    struct Attribute {
        enum class Type {
            float1, float2, float3, float4,
        };

        uint32_t bufferSlot;
        Type type;
        uint32_t location;
        uint32_t offset;
    };

    uint32_t slot;
    uint32_t size;
    std::vector<Attribute> attributes;
};

#endif //POMEGRANATE_VERTEX_H
