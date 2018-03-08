
cbuffer buffer : register(b0)
{
	float4x4 thomas_MatrixVP;
	float4x4 thomas_ObjectToWorld;
	float4 cameraPos;
	int gridScale;

};

struct VS_IN
{
	float3 Pos : POSITION;
	float viewDistance : COLOR1;
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
	DestBlend = INV_SRC_ALPHA;
	SrcBlend = SRC_ALPHA;
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
	VS_OUT output = (VS_OUT)0;
	output.Pos = mul(thomas_MatrixVP, mul(thomas_ObjectToWorld, float4(input.Pos, 1.0)));
	float4 positionW = mul(thomas_ObjectToWorld, float4(input.Pos, 1.0));
	float2 dist = distance(positionW.xz, cameraPos.xz) * 1 / input.viewDistance;
	dist -= pow(cameraPos.y + 1, 1.5f);
	dist += cameraPos.y*(1 - input.viewDistance)*50000;
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