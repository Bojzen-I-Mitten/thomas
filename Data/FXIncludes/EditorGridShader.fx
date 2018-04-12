
#include <ThomasCG.hlsl>

cbuffer buffer
{
	float4 cameraPos;
	int gridScale;
};

struct VS_IN
{
	float4 PosAndDistance : POSITION;
	float4 Color : COLOR0;
};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
	DepthFunc = LESS_EQUAL;
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

RasterizerState TestRasterizer
{
	FillMode = SOLID;
	CullMode = NONE;
	FrontCounterClockWise = TRUE;
	DepthClipEnable = FALSE;
	MultisampleEnable = TRUE;

};


VS_OUT VSMain(VS_IN input)
{
	float3 pos = input.PosAndDistance.xyz;
	float3 viewDistance = input.PosAndDistance.w;
	VS_OUT output = (VS_OUT)0;

	output.Pos = ThomasObjectToClipPos(pos);
	float4 positionW = ThomasObjectToWorldPos(pos);
	float2 dist = distance(positionW.xz, cameraPos.xz) * 1 / viewDistance;
	dist -= pow(cameraPos.y + 1, 1.5f);
	dist /= gridScale;
	output.Color = input.Color;
	output.Color.w = 1.0f - (dist / 10.0f);
	return output;
}

float4 PSMain(VS_OUT input) : SV_Target
{
	return input.Color;
}


technique11 Standard {
	pass P0 {
		SetVertexShader(CompileShader(vs_5_0, VSMain()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PSMain()));
		SetDepthStencilState(EnableDepth, 0);
		SetRasterizerState(TestRasterizer);
		SetBlendState(AlphaBlendingOn, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}