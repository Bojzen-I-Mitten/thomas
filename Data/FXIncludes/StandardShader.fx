#pragma warning(disable: 4717) // removes effect deprecation warning.

#include <ThomasCG.hlsl>

cbuffer MATERIAL_PROPERTIES
{
	float testMat;
	float4 wow;
};

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


float Fresnel(float lightAngle, float mediumRefractionIndex)
{
	float mediumTheta = 1.0f / mediumRefractionIndex;
	float refractAngle = sqrt(1 - mediumTheta * mediumTheta);

	float mediumLightAngleFactor = mediumRefractionIndex * lightAngle;
	float mediumRefractAngleFactor = mediumRefractionIndex* refractAngle;

	float Rparl = (mediumLightAngleFactor - refractAngle) / (mediumLightAngleFactor + refractAngle);
	float Rperp = (lightAngle - mediumRefractAngleFactor) / (lightAngle + mediumRefractAngleFactor);

	return (Rparl * Rparl + Rperp * Rperp) / 2;
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
	float4 testLightDir = normalize(float4(1, -1, -1, 0));
	float4 tempLightColorWith = float4(1.0f, 1.0f, 1.0f, 1.0f);

	float4 specular = float4(0, 0, 0, 0);
	float4 color = float4(0.2, 0.2, 0.2, 1.0f);
	
	float3 viewDir = i.worldPos - _WorldSpaceCameraPos;
	float lightIntensity = saturate(dot(-i.normal, testLightDir));
	

	
	color += saturate(tempLightColorWith * lightIntensity);
		//float4 reflectVec = dot(GetReflectVec(testLightDir, -i.normal), i.normal);
		//specular = saturate(dot(reflectVec, viewDir)) * Fresnel(lightIntensity, 1.3f);
	
	//color = saturate(color + specular);
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