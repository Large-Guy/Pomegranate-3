struct Input
{
    float3 Position : TEXCOORD0;
    float4 Color : TEXCOORD1;
};

struct Output
{
    float4 Color : TEXCOORD0;
    float4 Position : SV_Position;
};

Output _vertex(Input input)
{
    Output output;
    output.Position = float4(input.Position, 1.0f);
    output.Color = input.Color;
    return output;
}

float4 _fragment(Output output) : SV_TARGET
{
    return output.Color;
}
