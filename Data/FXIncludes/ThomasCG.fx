#ifndef THOMAS_CG_INCLUDED
#define THOMAS_CG_INCLUDED

#define THOMAS_PI            3.14159265359f
#define THOMAS_TWO_PI        6.28318530718f
#define THOMAS_FOUR_PI       12.56637061436f
#define THOMAS_INV_PI        0.31830988618f
#define THOMAS_INV_TWO_PI    0.15915494309f
#define THOMAS_INV_FOUR_PI   0.07957747155f
#define THOMAS_HALF_PI       1.57079632679f
#define THOMAS_INV_HALF_PI   0.636619772367f



#define VERT(vertex_shader) SetVertexShader( CompileShader( vs_5_0, vertex_shader) );
#define FRAG(fragment_shader) SetPixelShader( CompileShader( ps_5_o, fragment_shader) );


#include "ThomasShaderVariables.fx"
#include "ThomasShaderUtilities.fx"

#ifdef THOMAS_COLORSPACE_GAMMA
#define thomas_ColorSpaceGrey fixed4(0.5, 0.5, 0.5, 0.5)
#define thomas_ColorSpaceDouble fixed4(2.0, 2.0, 2.0, 2.0)
#define thomas_ColorSpaceDielectricSpec half4(0.220916301, 0.220916301, 0.220916301, 1.0 - 0.220916301)
#define thomas_ColorSpaceLuminance half4(0.22, 0.707, 0.071, 0.0) // Legacy: alpha is set to 0.0 to specify gamma mode
#else // Linear values
#define thomas_ColorSpaceGrey fixed4(0.214041144, 0.214041144, 0.214041144, 0.5)
#define thomas_ColorSpaceDouble fixed4(4.59479380, 4.59479380, 4.59479380, 2.0)
#define thomas_ColorSpaceDielectricSpec half4(0.04, 0.04, 0.04, 1.0 - 0.04) // standard dielectric reflectivity coef at incident angle (= 4%)
#define thomas_ColorSpaceLuminance half4(0.0396819152, 0.458021790, 0.00609653955, 1.0) // Legacy: alpha is set to 1.0 to specify linear mode
#endif

// -------------------------------------------------------------------
//  helper functions and macros used in many standard shaders


#define SCALED_NORMAL v.normal


// These constants must be kept in sync with RGBMRanges.h
#define LIGHTMAP_RGBM_SCALE 5.0
#define EMISSIVE_RGBM_SCALE 97.0


struct appdata_thomas {
	float3 vertex : POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
};


struct appdata_base {
    float4 vertex : POSITION;
    float3 normal : NORMAL;
    float4 texcoord : TEXCOORD0;
};

struct appdata_tan {
    float4 vertex : POSITION;
    float4 tangent : TANGENT;
    float3 normal : NORMAL;
    float4 texcoord : TEXCOORD0;
};

struct appdata_full {
    float4 vertex : POSITION;
    float4 tangent : TANGENT;
    float3 normal : NORMAL;
    float4 texcoord : TEXCOORD0;
    float4 texcoord1 : TEXCOORD1;
    float4 texcoord2 : TEXCOORD2;
    float4 texcoord3 : TEXCOORD3;
    fixed4 color : COLOR;
};


inline float GammaToLinearSpaceExact (float value)
{
    if (value <= 0.04045F)
        return value / 12.92F;
    else if (value < 1.0F)
        return pow((value + 0.055F)/1.055F, 2.4F);
    else
        return pow(value, 2.2F);
}

inline half3 GammaToLinearSpace (half3 sRGB)
{
    // Approximate version from http://chilliant.blogspot.com.au/2012/08/srgb-approximations-for-hlsl.html?m=1
    return sRGB * (sRGB * (sRGB * 0.305306011h + 0.682171111h) + 0.012522878h);

    // Precise version, useful for debugging.
    //return half3(GammaToLinearSpaceExact(sRGB.r), GammaToLinearSpaceExact(sRGB.g), GammaToLinearSpaceExact(sRGB.b));
}

inline float LinearToGammaSpaceExact (float value)
{
    if (value <= 0.0F)
        return 0.0F;
    else if (value <= 0.0031308F)
        return 12.92F * value;
    else if (value < 1.0F)
        return 1.055F * pow(value, 0.4166667F) - 0.055F;
    else
        return pow(value, 0.45454545F);
}

inline half3 LinearToGammaSpace (half3 linRGB)
{
    linRGB = max(linRGB, half3(0.h, 0.h, 0.h));
    // An almost-perfect approximation from http://chilliant.blogspot.com.au/2012/08/srgb-approximations-for-hlsl.html?m=1
    return max(1.055h * pow(linRGB, 0.416666667h) - 0.055h, 0.h);

    // Exact version, useful for debugging.
    //return half3(LinearToGammaSpaceExact(linRGB.r), LinearToGammaSpaceExact(linRGB.g), LinearToGammaSpaceExact(linRGB.b));
}

// Tranforms position from world to homogenous space
inline float4 ThomasWorldToClipPos( in float3 pos )
{
    return mul(THOMAS_MATRIX_VP, float4(pos, 1.0));
}

// Tranforms position from view to homogenous space
inline float4 ThomasViewToClipPos( in float3 pos )
{
    return mul(THOMAS_MATRIX_P, float4(pos, 1.0));
}

// Tranforms position from object to camera space
inline float3 ThomasObjectToViewPos( in float3 pos )
{
    return mul(THOMAS_MATRIX_V, mul(thomas_ObjectToWorld, float4(pos, 1.0))).xyz;
}
inline float3 ThomasObjectToViewPos(float4 pos) // overload for float4; avoids "implicit truncation" warning for existing shaders
{
    return ThomasObjectToViewPos(pos.xyz);
}

// Tranforms position from world to camera space
inline float3 ThomasWorldToViewPos( in float3 pos )
{
    return mul(THOMAS_MATRIX_V, float4(pos, 1.0)).xyz;
}

// Transforms direction from object to world space
inline float3 ThomasObjectToWorldDir( in float3 dir )
{
    return normalize(mul((float3x3)thomas_ObjectToWorld, dir));
}

// Transforms direction from world to object space
inline float3 ThomasWorldToObjectDir( in float3 dir )
{
    return normalize(mul((float3x3)thomas_WorldToObject, dir));
}

// Transforms normal from object to world space
inline float3 ThomasObjectToWorldNormal( in float3 norm )
{
#ifdef THOMAS_ASSUME_UNIFORM_SCALING
    return ThomasObjectToWorldDir(norm);
#else
    // mul(IT_M, norm) => mul(norm, I_M) => {dot(norm, I_M.col0), dot(norm, I_M.col1), dot(norm, I_M.col2)}
    return normalize(mul(norm, (float3x3)thomas_WorldToObject));
#endif
}

// Computes world space light direction, from world space position
inline float3 ThomasWorldSpaceLightDir( in float3 worldPos )
{
    #ifndef USING_LIGHT_MULTI_COMPILE
        return _WorldSpaceLightPos0.xyz - worldPos * _WorldSpaceLightPos0.w;
    #else
        #ifndef USING_DIRECTIONAL_LIGHT
        return _WorldSpaceLightPos0.xyz - worldPos;
        #else
        return _WorldSpaceLightPos0.xyz;
        #endif
    #endif
}

// Computes object space light direction
inline float3 ObjSpaceLightDir( in float4 v )
{
    float3 objSpaceLightPos = mul(thomas_WorldToObject, _WorldSpaceLightPos0).xyz;
    #ifndef USING_LIGHT_MULTI_COMPILE
        return objSpaceLightPos.xyz - v.xyz * _WorldSpaceLightPos0.w;
    #else
        #ifndef USING_DIRECTIONAL_LIGHT
        return objSpaceLightPos.xyz - v.xyz;
        #else
        return objSpaceLightPos.xyz;
        #endif
    #endif
}

// Computes world space view direction, from object space position
inline float3 ThomasWorldSpaceViewDir( in float3 worldPos )
{
    return _WorldSpaceCameraPos.xyz - worldPos;
}

// Computes world space view direction, from object space position
// *Legacy* Please use ThomasWorldSpaceViewDir instead
inline float3 WorldSpaceViewDir( in float4 localPos )
{
    float3 worldPos = mul(thomas_ObjectToWorld, localPos).xyz;
    return ThomasWorldSpaceViewDir(worldPos);
}

// Computes object space view direction
inline float3 ObjSpaceViewDir( in float4 v )
{
    float3 objSpaceCameraPos = mul(thomas_WorldToObject, float4(_WorldSpaceCameraPos.xyz, 1)).xyz;
    return objSpaceCameraPos - v.xyz;
}

// Declares 3x3 matrix 'rotation', filled with tangent space basis
#define TANGENT_SPACE_ROTATION \
    float3 binormal = cross( normalize(v.normal), normalize(v.tangent.xyz) ) * v.tangent.w; \
    float3x3 rotation = float3x3( v.tangent.xyz, binormal, v.normal )



// Used in ForwardBase pass: Calculates diffuse lighting from 4 point lights, with data packed in a special way.
float3 Shade4PointLights (
    float4 lightPosX, float4 lightPosY, float4 lightPosZ,
    float3 lightColor0, float3 lightColor1, float3 lightColor2, float3 lightColor3,
    float4 lightAttenSq,
    float3 pos, float3 normal)
{
    // to light vectors
    float4 toLightX = lightPosX - pos.x;
    float4 toLightY = lightPosY - pos.y;
    float4 toLightZ = lightPosZ - pos.z;
    // squared lengths
    float4 lengthSq = 0;
    lengthSq += toLightX * toLightX;
    lengthSq += toLightY * toLightY;
    lengthSq += toLightZ * toLightZ;
    // don't produce NaNs if some vertex position overlaps with the light
    lengthSq = max(lengthSq, 0.000001);

    // NdotL
    float4 ndotl = 0;
    ndotl += toLightX * normal.x;
    ndotl += toLightY * normal.y;
    ndotl += toLightZ * normal.z;
    // correct NdotL
    float4 corr = rsqrt(lengthSq);
    ndotl = max (float4(0,0,0,0), ndotl * corr);
    // attenuation
    float4 atten = 1.0 / (1.0 + lengthSq * lightAttenSq);
    float4 diff = ndotl * atten;
    // final color
    float3 col = 0;
    col += lightColor0 * diff.x;
    col += lightColor1 * diff.y;
    col += lightColor2 * diff.z;
    col += lightColor3 * diff.w;
    return col;
}

// Used in Vertex pass: Calculates diffuse lighting from lightCount lights. Specifying true to spotLight is more expensive
// to calculate but lights are treated as spot lights otherwise they are treated as point lights.
float3 ShadeVertexLightsFull (float4 vertex, float3 normal, int lightCount, bool spotLight)
{
    float3 viewpos = ThomasObjectToViewPos (vertex);
    float3 viewN = normalize (mul ((float3x3)THOMAS_MATRIX_IT_MV, normal));

    float3 lightColor = THOMAS_LIGHTMODEL_AMBIENT.xyz;
    for (int i = 0; i < lightCount; i++) {
        float3 toLight = thomas_LightPosition[i].xyz - viewpos.xyz * thomas_LightPosition[i].w;
        float lengthSq = dot(toLight, toLight);

        // don't produce NaNs if some vertex position overlaps with the light
        lengthSq = max(lengthSq, 0.000001);

        toLight *= rsqrt(lengthSq);

        float atten = 1.0 / (1.0 + lengthSq * thomas_LightAtten[i].z);
        if (spotLight)
        {
            float rho = max (0, dot(toLight, thomas_SpotDirection[i].xyz));
            float spotAtt = (rho - thomas_LightAtten[i].x) * thomas_LightAtten[i].y;
            atten *= saturate(spotAtt);
        }

        float diff = max (0, dot (viewN, toLight));
        lightColor += thomas_LightColor[i].rgb * (diff * atten);
    }
    return lightColor;
}

float3 ShadeVertexLights (float4 vertex, float3 normal)
{
    return ShadeVertexLightsFull (vertex, normal, 4, false);
}

// normal should be normalized, w=1.0
half3 SHEvalLinearL0L1 (half4 normal)
{
    half3 x;

    // Linear (L1) + constant (L0) polynomial terms
    x.r = dot(thomas_SHAr,normal);
    x.g = dot(thomas_SHAg,normal);
    x.b = dot(thomas_SHAb,normal);

    return x;
}

// normal should be normalized, w=1.0
half3 SHEvalLinearL2 (half4 normal)
{
    half3 x1, x2;
    // 4 of the quadratic (L2) polynomials
    half4 vB = normal.xyzz * normal.yzzx;
    x1.r = dot(thomas_SHBr,vB);
    x1.g = dot(thomas_SHBg,vB);
    x1.b = dot(thomas_SHBb,vB);

    // Final (5th) quadratic (L2) polynomial
    half vC = normal.x*normal.x - normal.y*normal.y;
    x2 = thomas_SHC.rgb * vC;

    return x1 + x2;
}

// normal should be normalized, w=1.0
// output in active color space
half3 ShadeSH9 (half4 normal)
{
    // Linear + constant polynomial terms
    half3 res = SHEvalLinearL0L1 (normal);

    // Quadratic polynomials
    res += SHEvalLinearL2 (normal);

#   ifdef THOMAS_COLORSPACE_GAMMA
        res = LinearToGammaSpace (res);
#   endif

    return res;
}

// OBSOLETE: for backwards compatibility with 5.0
half3 ShadeSH3Order(half4 normal)
{
    // Quadratic polynomials
    half3 res = SHEvalLinearL2 (normal);

#   ifdef THOMAS_COLORSPACE_GAMMA
        res = LinearToGammaSpace (res);
#   endif

    return res;
}

#if THOMAS_LIGHT_PROBE_PROXY_VOLUME

// normal should be normalized, w=1.0
half3 SHEvalLinearL0L1_SampleProbeVolume (half4 normal, float3 worldPos)
{
    const float transformToLocal = thomas_ProbeVolumeParams.y;
    const float texelSizeX = thomas_ProbeVolumeParams.z;

    //The SH coefficients textures and probe occlusion are packed into 1 atlas.
    //-------------------------
    //| ShR | ShG | ShB | Occ |
    //-------------------------

    float3 position = (transformToLocal == 1.0f) ? mul(thomas_ProbeVolumeWorldToObject, float4(worldPos, 1.0)).xyz : worldPos;
    float3 texCoord = (position - thomas_ProbeVolumeMin.xyz) * thomas_ProbeVolumeSizeInv.xyz;
    texCoord.x = texCoord.x * 0.25f;

    // We need to compute proper X coordinate to sample.
    // Clamp the coordinate otherwize we'll have leaking between RGB coefficients
    float texCoordX = clamp(texCoord.x, 0.5f * texelSizeX, 0.25f - 0.5f * texelSizeX);

    // sampler state comes from SHr (all SH textures share the same sampler)
    texCoord.x = texCoordX;
    half4 SHAr = THOMAS_SAMPLE_TEX3D_SAMPLER(thomas_ProbeVolumeSH, thomas_ProbeVolumeSH, texCoord);

    texCoord.x = texCoordX + 0.25f;
    half4 SHAg = THOMAS_SAMPLE_TEX3D_SAMPLER(thomas_ProbeVolumeSH, thomas_ProbeVolumeSH, texCoord);

    texCoord.x = texCoordX + 0.5f;
    half4 SHAb = THOMAS_SAMPLE_TEX3D_SAMPLER(thomas_ProbeVolumeSH, thomas_ProbeVolumeSH, texCoord);

    // Linear + constant polynomial terms
    half3 x1;
    x1.r = dot(SHAr, normal);
    x1.g = dot(SHAg, normal);
    x1.b = dot(SHAb, normal);

    return x1;
}
#endif

// normal should be normalized, w=1.0
half3 ShadeSH12Order (half4 normal)
{
    // Linear + constant polynomial terms
    half3 res = SHEvalLinearL0L1 (normal);

#   ifdef THOMAS_COLORSPACE_GAMMA
        res = LinearToGammaSpace (res);
#   endif

    return res;
}

// Transforms 2D UV by scale/bias property
#define TRANSFORM_TEX(tex,name) (tex.xy * name##_ST.xy + name##_ST.zw)



struct v2f_vertex_lit {
    float2 uv   : TEXCOORD0;
    fixed4 diff : COLOR0;
    fixed4 spec : COLOR1;
};

inline fixed4 VertexLight( v2f_vertex_lit i, sampler2D mainTex )
{
    fixed4 texcol = tex2D( mainTex, i.uv );
    fixed4 c;
    c.xyz = ( texcol.xyz * i.diff.xyz + i.spec.xyz * texcol.a );
    c.w = texcol.w * i.diff.w;
    return c;
}


// Calculates UV offset for parallax bump mapping
inline float2 ParallaxOffset( half h, half height, half3 viewDir )
{
    h = h * height - height/2.0;
    float3 v = normalize(viewDir);
    v.z += 0.42;
    return h * (v.xy / v.z);
}

// Converts color to luminance (grayscale)
inline half Luminance(half3 rgb)
{
    return dot(rgb, thomas_ColorSpaceLuminance.rgb);
}

// Convert rgb to luminance
// with rgb in linear space with sRGB primaries and D65 white point
half LinearRgbToLuminance(half3 linearRgb)
{
    return dot(linearRgb, half3(0.2126729f,  0.7151522f, 0.0721750f));
}

half4 ThomasEncodeRGBM (half3 color, float maxRGBM)
{
    float kOneOverRGBMMaxRange = 1.0 / maxRGBM;
    const float kMinMultiplier = 2.0 * 1e-2;

    float3 rgb = color * kOneOverRGBMMaxRange;
    float alpha = max(max(rgb.r, rgb.g), max(rgb.b, kMinMultiplier));
    alpha = ceil(alpha * 255.0) / 255.0;

    // Division-by-zero warning from d3d9, so make compiler happy.
    alpha = max(alpha, kMinMultiplier);

    return half4(rgb / alpha, alpha);
}

// Decodes HDR textures
// handles dLDR, RGBM formats
inline half3 DecodeHDR (half4 data, half4 decodeInstructions)
{
    // Take into account texture alpha if decodeInstructions.w is true(the alpha value affects the RGB channels)
    half alpha = decodeInstructions.w * (data.a - 1.0) + 1.0;

    // If Linear mode is not supported we can skip exponent part
    #if defined(THOMAS_COLORSPACE_GAMMA)
        return (decodeInstructions.x * alpha) * data.rgb;
    #else
    #   if defined(THOMAS_USE_NATIVE_HDR)
            return decodeInstructions.x * data.rgb; // Multiplier for future HDRI relative to absolute conversion.
    #   else
            return (decodeInstructions.x * pow(alpha, decodeInstructions.y)) * data.rgb;
    #   endif
    #endif
}


#endif // THOMAS_CG_INCLUDED
