#pragma warning(disable: 4717) // removes effect deprecation warning.

#include <ThomasCG.hlsl>
Texture2D ambientTex;


SamplerState StandardWrapSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

cbuffer MATERIAL_PROPERTIES
{
    
	float4 wow : COLOR;
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

float3 GetHalfwayVec(float3 lightDir, float3 viewDir)
{
    return normalize(viewDir + lightDir);
}

struct v2f {
	float4 vertex : SV_POSITION;
	float4 worldPos : POSITIONWS;
	float3 normal : NORMAL;
    float2 texcoord : TEXCOORD0;
};

v2f vert(appdata_thomas v)
{
	v2f o;
	o.vertex = ThomasObjectToClipPos(v.vertex);
	o.worldPos = ThomasObjectToWorldPos(v.vertex);
	o.normal = ThomasWorldToObjectDir(v.normal);
    o.texcoord = v.texcoord;
	return o;
}

/*struct Light
{
    float4  color;
    float4  position;
    float4  direction;
    float   intensity;
    float   smoothness; //specularIntensity
    float   spotInnerAngle;
    float   spotOuterAngle;
    float3  attenuation;
    uint    type;
};*/

struct ConstantBufferForLights
{
    uint nrOfPointLights;
    uint nrOfSpotLights;
    uint nrOfDirectionalLights;
    uint pad;
};

struct Light
{
	float3  color;
	float   intensity;
	float3  position;
	float   spotOuterAngle;
	float3  direction;
	float   spotInnerAngle;
	float3  attenuation;
    uint type;

    
};

//StructuredBuffer<Light> lights;

float CalculatePointLightContribution(float4 lightColor, float lightIntensity, float lightDistance, float3 lightAttenuation)
{
	return lightColor * lightIntensity / (lightAttenuation.x + lightAttenuation.y * lightDistance + lightAttenuation.z * lightDistance * lightDistance);
}

/*float CalculateSpotLightFactor()
{
	float angle = degrees(acos(dot(-tempLight.direction, lightDir)));
	float spotFactor = 0.0f;
	if (angle < tempLight.spotInnerAngle)
	{
		spotFactor = 1.0f;
	}
	else if (angle < tempLight.spotOuterAngle)
	{
		spotFactor = 1.0f - smoothstep(tempLight.spotInnerAngle, tempLight.spotOuterAngle, angle);
	}
	return spotFactor;
}*/

void Apply(inout float4 colorAcculmulator, float3 lightMultiplyer, float3 normal, float3 lightDir, float3 viewDir)//should take material properties later
{
    float4 ambient = float4(0.1f, 0.1f, 0.1f, 1.0f);
    float4 diffuse = float4(0.4f, 0.4f, 0.4f, 1.0f);
    float4 specular = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float smoothness = 16.0f;

    float lambertian = saturate(dot(normal, lightDir));
    float specularIntensity = 0.0f;
    if (lambertian > 0.0f)
    {
        specularIntensity = pow(saturate(dot(normal, GetHalfwayVec(viewDir, lightDir))), smoothness); //blinn-phong
    }
    
    colorAcculmulator += ambient + (diffuse * lambertian + specular * specularIntensity) * float4(lightMultiplyer, 1);
}

float4 frag(v2f input) : SV_TARGET
{

    Light tempLight;
    tempLight.color = float3(0.5f, 0.5f, 0.5f);
    tempLight.position = float3(3, 3, 3);
    tempLight.intensity = 1;
    tempLight.direction = -normalize(float3(1, 1, 1));
    tempLight.spotInnerAngle = 10.0f;
    tempLight.spotOuterAngle = 30.0f;
    tempLight.attenuation = float3(0.4f, 0.02f, 0.1f);
    tempLight.type = 0;

    ConstantBufferForLights testCBuffer;
    testCBuffer.nrOfDirectionalLights = 1;
    testCBuffer.nrOfPointLights = 0;
    testCBuffer.nrOfSpotLights = 0;
    
    
    float4 finalColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float3 viewDir = normalize(_WorldSpaceCameraPos - input.worldPos.xyz);
	float3 lightDir = float3(0, 0, 0);
    float3 lightMultiplyer = float3(0.0f, 0.0f, 0.0f);
    
    int i = 0;
    int roof = testCBuffer.nrOfDirectionalLights;
    for (; i < roof; ++i) //directional
    {
        lightDir = -tempLight.direction;
        lightMultiplyer = tempLight.color * tempLight.intensity;
        Apply(finalColor, lightMultiplyer, input.normal, lightDir, viewDir);
    }
    roof += testCBuffer.nrOfPointLights;
    for (; i < roof; ++i) //point
    {
        lightDir = tempLight.position - input.worldPos.xyz;
        float lightDistance = length(lightDir);
        lightDir = normalize(lightDir);

        lightMultiplyer = tempLight.color * tempLight.intensity / (tempLight.attenuation.x + tempLight.attenuation.y * lightDistance + tempLight.attenuation.z * lightDistance * lightDistance);
        Apply(finalColor, lightMultiplyer, input.normal, lightDir, viewDir);
    }
    roof += testCBuffer.nrOfSpotLights;
    for (; i < roof; ++i) //spot
    {
        lightDir = tempLight.position - input.worldPos.xyz;
        float lightDistance = length(lightDir);
        lightDir = normalize(lightDir);

        float angle = degrees(acos(dot(-tempLight.direction, lightDir)));
        float spotFactor = 0.0f;
        if (angle < tempLight.spotInnerAngle)
        {
            spotFactor = 1.0f;
        }
        else if (angle < tempLight.spotOuterAngle)
        {
            spotFactor = 1.0f - smoothstep(tempLight.spotInnerAngle, tempLight.spotOuterAngle, angle);
        }

        lightMultiplyer = spotFactor * tempLight.color * tempLight.intensity / (tempLight.attenuation.x + tempLight.attenuation.y * lightDistance + tempLight.attenuation.z * lightDistance * lightDistance);
        Apply(finalColor, lightMultiplyer, input.normal, lightDir, viewDir);
    }
    return saturate(ambientTex.Sample(StandardWrapSampler, input.texcoord) * wow + finalColor);
  //  return saturate(finalColor);

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