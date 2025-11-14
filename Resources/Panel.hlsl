struct Input
{
    uint id : SV_VertexID;
};

struct Output
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float2 pixelUV : TEXCOORD1;
    float4 color : TEXCOORD2;
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
    output.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    output.position = float4(position, 0.0f, 1.0f);
    output.uv = vertices[input.id];
    output.pixelUV = vertices[input.id] * vlayout.rect.zw;
    return output;
}

float rand(float2 n)
{
    return frac(sin(dot(n, float2(12.9898, 78.233))) * 43758.5453);
}

float3 hsv(in float3 input)
{
    float4 K = float4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    float3 p = abs(frac(input.xxx + K.xyz) * 6.0 - K.www);

    return input.z * lerp(K.xxx, clamp(p - K.xxx, 0.0, 1.0), input.y);
}

float sdfRoundedBox(in float2 p, in float2 b, in float4 r)
{
    p -= b * 0.5f;
    p *= 2.0f;
    r.xy = (p.x > 0.0f) ? r.xy : r.zw;
    r.x = (p.y > 0.0f) ? r.x : r.y;
    float2 q = abs(p) - b + r.x;
    return min(max(q.x, q.y), 0.0f) + length(max(q, 0.0f)) - r.x;
}

bool edging(Output input, float rounding, float size)
{
    bool q = sdfRoundedBox(input.pixelUV, flayout.rect.zw, rounding) < 0.0f;
    bool q1 = sdfRoundedBox(input.pixelUV + float2(size, 0.0), flayout.rect.zw, rounding) < 0.0f;
    bool q2 = sdfRoundedBox(input.pixelUV + float2(-size, 0.0), flayout.rect.zw, rounding) < 0.0f;
    bool q3 = sdfRoundedBox(input.pixelUV + float2(0.0, size), flayout.rect.zw, rounding) < 0.0f;
    bool q4 = sdfRoundedBox(input.pixelUV + float2(0.0, -size), flayout.rect.zw, rounding) < 0.0f;

    return q != q1 || q != q2 || q != q3 || q != q4;
}

float clamp01(float v)
{
    return clamp(v, 0.0f, 1.0f);
}

float4 _fragment(Output input) : SV_TARGET
{
    float4 color = float4(input.uv.xy, 0.0f, 1.0f);
    float rounding = 24.0f;
    float minBreadth = min(flayout.rect.z, flayout.rect.w);
    rounding = min(rounding, minBreadth);
    float sdfQuery = sdfRoundedBox(input.pixelUV, flayout.rect.zw, rounding);
    float r = rand(flayout.rect.xy);
    float3 randColor = normalize(float3(rand(flayout.rect.xy), rand(flayout.rect.xy + float2(1.0f,0.0f)), rand(flayout.rect.xy + float2(0.0f, 1.0f))));

    if(sdfQuery < 0.0f)
    {
        float shadow = 8;

        bool edge = edging(input, rounding, 1.0f);
        if(edge)
        {
            color.xyz = float3(0.2f,0.2f,0.2f);
        }
        else
        {
            color.xyz = lerp(float3(0.125f,0.125f,0.125f), float3(0.15f, 0.15f, 0.15f), clamp01((flayout.rect.w - input.pixelUV.y) / shadow));
        }
    }
    else
    {
        color.a = 0.0f;
    }
    return color;
}
