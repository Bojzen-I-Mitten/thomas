#ifndef THOMAS_SHADER_UTILITIES_INCLUDED
#define THOMAS_SHADER_UTILITIES_INCLUDED

// This file is always included in all thomas shaders.

#include "ThomasShaderVariables.fx"

// Tranforms position from object to homogenous space
inline float4 ThomasObjectToClipPos(in float3 pos)
{
    // More efficient than computing M*VP matrix product
    return mul(THOMAS_MATRIX_VP, mul(thomas_ObjectToWorld, float4(pos, 1.0)));
}
inline float4 ThomasObjectToClipPos(float4 pos) // overload for float4; avoids "implicit truncation" warning for existing shaders
{
    return ThomasObjectToClipPos(pos.xyz);
}

#endif
