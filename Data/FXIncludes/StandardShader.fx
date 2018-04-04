#pragma warning(disable: 4717) // removes effect deprecation warning.

#include "ThomasCG.fx"


DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
	DepthFunc = LESS_EQUAL;
};

RasterizerState TestRasterizer
{
	FillMode = SOLID;
	CullMode = BACK;
	FrontCounterClockWise = TRUE;
	DepthClipEnable = FALSE;
};


BlendState AlphaBlendingOn
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;

};

float3 GetReflectVec(float3 inVec, float3 normal)
{
	return 2 * dot(normal, inVec)*normal - inVec;
}

float4 GetReflectVec(float4 inVec, float4 normal)
{
	return 2 * dot(normal, inVec)*normal - inVec;
}

float4 GetHalfwayVec(float4 lightDir, float4 viewDir)
{
	return normalize(lightDir + viewDir);
}

struct v2f {
	float4 vertex : SV_POSITION;
	float4 worldPos : POSITIONWS;
	float4 normal : NORMAL;
};

v2f vert(appdata_thomas v)
{
	v2f o;
	o.vertex = ThomasObjectToClipPos(v.vertex);
	o.worldPos = ThomasObjectToWorldPos(v.vertex);
	o.normal = float4(ThomasWorldToObjectDir(v.normal), 0);
	return o;
}

struct Light
{
    float4 color;
    float4 position;
    float4 direction;
    float intensity;
    float range;
    uint type;
    bool drawHalo;
};

float4 frag(v2f input) : SV_TARGET
{

    Light tempLight;
    tempLight.color = float4(0.5f, 0.5f, 0.5f, 1.0f);
    tempLight.position = float4(5, 5, 5, 1);
    tempLight.range = 7;
    tempLight.direction = normalize(float4(1, 1, 1, 0));
    tempLight.type = 1;
    
    //colors
    float4 ambient = float4(0.2, 0.2, 0.2, 1.0f);
    float4 diffuse = float4(0.5f, 0.5f, 0.5f, 1.0f);
    float4 specular = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    float4 viewDir = float4(input.worldPos.xyz - _WorldSpaceCameraPos, 0.0f);
    float4 lightDir = float4(0, 0, 0, 0);
    float lightMultiplyer = 1.0f;

    if (0 == tempLight.type)
    {
        lightDir = tempLight.direction;
        lightMultiplyer = tempLight.color;
    }
    else if (1 == tempLight.type)
    {
        lightDir = tempLight.position - input.worldPos;
        float lightDistance = length(lightDir);
        lightDir = normalize(lightDir);
        lightDistance *= lightDistance;
        lightMultiplyer = tempLight.color * tempLight.range / lightDistance;
    }
    

    float lambertian = saturate(dot(input.normal, lightDir));
    float specularIntensity = 0.0f;
    if (lambertian > 0.0f)
    {
        specularIntensity = pow(saturate(dot(input.normal, GetHalfwayVec(viewDir, lightDir))), 16.0f);
    }
    
    return saturate(ambient + (diffuse * lambertian + specular * specularIntensity) * lightMultiplyer);

}


technique11 Standard {
	pass P0 {
		VERT(vert());
		SetGeometryShader(NULL);
		FRAG(frag());
		SetDepthStencilState(EnableDepth, 0);
		SetRasterizerState(TestRasterizer);
		SetBlendState(AlphaBlendingOn, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}

}