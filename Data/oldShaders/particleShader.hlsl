Texture2D diffuseTexture : register(t0);
SamplerState diffuseSampler : register(s0);

cbuffer matrixBuffer : register(b0)
{
	matrix viewProjMatrix;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uvs : TEXCOORD0;
    float2 paddddd : padding;
    float4 colorFactor : COLOR0;
};

struct BillboardStruct
{
	float3 quad[2][3];
    float2 pad2;
	float2 uvs[2][3];
    float4 colorFactor;
};

StructuredBuffer<BillboardStruct> particle : register(t1);

VS_OUT  VSMain(uint id : SV_VERTEXID)
{
	VS_OUT output = (VS_OUT) 0;

	uint particleIndex = (uint)(id/6);
	uint triangleIndex = (uint)((id % 6)/3);
	uint vertexIndex = id % 3;

	output.pos = mul(float4(particle[particleIndex].quad[triangleIndex][vertexIndex], 1.0), viewProjMatrix);
	output.uvs = particle[particleIndex].uvs[triangleIndex][vertexIndex];
    output.colorFactor = particle[particleIndex].colorFactor;

	return output;
}


float4 PSMain(VS_OUT input) : SV_Target
{
    float4 outputColor = diffuseTexture.Sample(diffuseSampler, input.uvs);

    outputColor *= input.colorFactor;
    
	return outputColor;

}
