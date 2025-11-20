struct Input
{
    uint id : SV_VertexID;
};

struct Output
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float2 pixelUV : TEXCOORD1;
    float2 screenUV : TEXCOORD2;
};

struct Layout
{
    float4 screen;
    float4 rect;
};

cbuffer vert : register(b0, space1)
{
    Layout vlayout;
}

cbuffer frag : register(b0, space1)
{
    Layout flayout;
}

Texture2D<float4> texture : register(t0, space2);
SamplerState textureSampler : register(s0, space2);

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

    float2 position = vertices[input.id] * vlayout.rect.zw + vlayout.rect.xy + vlayout.screen.xy;

    position /= (vlayout.screen.zw / 2.0f);

    Output output;
    output.position = float4(position, 0.0f, 1.0f);
    output.uv = vertices[input.id];
    output.screenUV = (position + float2(1.0f, 1.0f)) * 0.5f;
    output.pixelUV = vertices[input.id] * vlayout.rect.zw;
    return output;
}

float4 _fragment(Output input) : SV_TARGET
{
    return texture.Sample(textureSampler, input.uv);
}
