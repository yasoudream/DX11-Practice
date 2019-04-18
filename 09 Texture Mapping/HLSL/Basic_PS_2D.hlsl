#include "Basic.hlsli"

// ÏñËØ×ÅÉ«Æ÷(2D)
float4 PS_2D(VertexPosHTex pIn) : SV_Target
{
    return g_TexArray.Sample(g_SamLinear, float3(pIn.Tex, g_fireFrame));
}