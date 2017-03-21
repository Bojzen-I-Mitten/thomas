
SamplerState ObjSamplerState : register(s10);
TextureCube SkyMap : register(t10);
TextureCube SkyMap2 : register(t11);

struct VSInput
{
	float3 position : POSITION;
};

cbuffer mvp : register(b0)
{
	matrix mvpMatrix;
	matrix viewMatrix;
	float3 camPosition;
	float padding;
    float3 rando;
    float padding3;
};

//Struct coupled with LightManager
struct DirLight
{
	float4 lightColor;
	float3 lightDir;
	float padding;
};
//Struct coupled with LightManager
struct PointLight
{
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
	float power;
	float4 lightColor;
	float3 position;
	float padding;
};
//Buffer coupled with LightManager
cbuffer lightBuffer : register(b2)
{
	uint nrOfDirectionalLights;
	uint nrOfPointLights;
	int padding1;
	int padding2;
	DirLight directionalLights[3];
	PointLight pointLights[20];
}



struct VSOutput
{
	float4 position : SV_POSITION;
	float3 texCoord : TEXCOORD;
};



VSOutput VSMain(in VSInput input)
{
	VSOutput output;

    output.position = mul(float4(input.position, 1), mvpMatrix).xyww;
	output.texCoord = mul(input.position,(float3x3)viewMatrix);
	return output;
}

float4 PSMain(VSOutput input) : SV_TARGET
{
    float4 cube1 = SkyMap.Sample(ObjSamplerState, input.texCoord);
    float4 cube2 = SkyMap2.Sample(ObjSamplerState, input.texCoord);
    cube2 = lerp(cube2, float4(1, 1, 1, 1), 0.5);
    float4 output = lerp(cube1, cube2, 0.3);
    return lerp(output, float4(rando, 1.f), 0.50f);
	//return SkyMap.Sample(ObjSamplerState, input.texCoord);
}