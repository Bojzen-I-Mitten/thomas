Texture2D backBufferTexture : register(t0);
SamplerState backBufferSampler : register(s0);

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};


VS_OUT VSMain( /*VS_IN input,*/uint id : SV_VERTEXID)
{
	VS_OUT output = (VS_OUT) 0;
	
	output.Pos = float4((id == 0 || id == 1) * 2 - 1, (id == 0 || id == 2) * 2 - 1, 0, 1);
	output.Tex = float2((id == 0 || id == 1), (id == 1 || id == 3));

	return output;
}

float4 PSMain(VS_OUT input) : SV_Target
{
	return backBufferTexture.Sample(backBufferSampler, input.Tex);
}