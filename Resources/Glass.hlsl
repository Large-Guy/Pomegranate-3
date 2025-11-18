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

Texture2D<float4> background : register(t0, space2);
SamplerState backgroundSampler : register(s0, space2);

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

float rand(float2 n)
{
    return frac(sin(dot(n, float2(12.9898, 78.233))) * 43758.5453);
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
    static const int samples = 8;
    static const float rounding = 24.0f;

    float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float total = 0.0;
    float radius = 0.03f;
    for(int i = 0; i < samples; i++)
    {
         float angle = float(i) * 2.39996323; //Golden ratio angle
         float dist = sqrt(float(i) / float(samples)) * radius;

         float2 offset = float2(cos(angle), sin(angle)) * dist;
         color += background.Sample(backgroundSampler, input.screenUV + offset);
    }

    color /= (float)samples;

    color = clamp(color, 0.0f, 1.0f);

    //color.r = background.Sample(backgroundSampler, input.screenUV + float2(radius * 0.5, 0.0)).r;
    //color.b = background.Sample(backgroundSampler, input.screenUV - float2(radius * 0.5, 0.0)).b;

    float sdfQuery = sdfRoundedBox(input.pixelUV, flayout.rect.zw, rounding);
    if(sdfQuery < 0.0f)
    {
        bool edge = edging(input, rounding, 4.0f);
        if(edge)
        {
            color += float4(0.2f, 0.2f, 0.2f, 1.0f);
        }
    }
    else
    {
        color.a = 0.0f;
    }

    return color;
}
