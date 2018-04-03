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

float4 frag(v2f i) : SV_TARGET
{
	float4 tempLightDir = normalize(float4(1, -1, -1, 0));
	float4 tempLightDiffuseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 tempLightSpecularColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//float tempDiffuseDistance = 5.0f;//diffuse power used for point lights

	float4 specular = float4(0, 0, 0, 0);
	float4 color = float4(0.2, 0.2, 0.2, 1.0f);
	
	float3 viewDir = i.worldPos - _WorldSpaceCameraPos;
    float diffuseIntensity = saturate(dot(-i.normal, tempLightDir));
	
	float specularIntensity = pow(saturate(dot(-i.normal, GetHalfwayVec(tempLightDir, float4(viewDir, 1.0f)))), 10.0f);

	color += saturate(tempLightDiffuseColor * diffuseIntensity + tempLightSpecularColor * specularIntensity);

	
	return color;
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