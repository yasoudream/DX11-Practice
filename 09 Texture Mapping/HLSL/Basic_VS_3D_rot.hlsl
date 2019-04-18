#include "Basic.hlsli"

// ¶¥µã×ÅÉ«Æ÷(3D)
VertexPosHWNormalTex VS_3D(VertexPosNormalTex vIn)
{
    VertexPosHWNormalTex vOut;
    matrix viewProj = mul(g_View, g_Proj);
    float4 posW = mul(float4(vIn.PosL, 1.0f), g_World);

    vOut.PosH = mul(posW, viewProj);
    vOut.PosW = posW.xyz;
    vOut.NormalW = mul(vIn.NormalL, (float3x3) g_WorldInvTranspose);

    float2x2 m = float2x2(rot.x, rot.y, -rot.y, rot.x);
    float2 n = vIn.Tex - (0.5f, 0.5f);
    float2 d = mul(m, n) + (0.5f, 0.5f);
    vOut.Tex = d;
    
    
    return vOut;
}
