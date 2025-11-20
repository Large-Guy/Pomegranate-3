struct Input
{
    uint id : SV_VertexID;
};

struct Output
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

Texture2D<float4> oldLayer : register(t0, space2);
Texture2D<float4> newLayer : register(t1, space2);
SamplerState oldSampler : register(s0, space2);
SamplerState newSampler : register(s1, space2);

Output _vertex(Input input)
{
    static const float2 tl = float2(0.0f, 0.0f);
    static const float2 tr = float2(1.0f, 0.0f);
    static const float2 bl = float2(0.0f, 1.0f);
    static const float2 br = float2(1.0f, 1.0f);

    static const float2 vertices[6] = {
        tl, tr, bl,
        bl, tr, br,
    };

    float2 position = vertices[input.id] * 2.0f - float2(1.0f, 1.0f);

    Output output;
    output.position = float4(position, 0.0f, 1.0f);
    output.uv = vertices[input.id];
    return output;
}

float4 _fragment(Output input) : SV_TARGET
{
    float4 under = oldLayer.Sample(oldSampler, input.uv);
    float4 over = newLayer.Sample(newSampler, input.uv);
    float4 result = lerp(under, over, over.a);
    //result.a = under.a + over.a * (1.0 - under.a);

    return result;
}
