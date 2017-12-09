
cbuffer buffer : register(b0)
{
	float4x4 thomas_MatrixVP;
	float4x4 thomas_ObjectToWorld;
};

struct VS_IN
{
	float3 Pos : POSITION;
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

RasterizerState TestRasterizer
{
	FillMode = SOLID;
	CullMode = NONE;
	FrontCounterClockWise = TRUE;
	DepthClipEnable = FALSE;
};



VS_OUT VSMain(VS_IN input)
{
	VS_OUT output = (VS_OUT) 0;
    output.Pos = mul(thomas_MatrixVP, mul(thomas_ObjectToWorld, float4(input.Pos, 1.0)));
	output.Color = input.Color;

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
	}
}