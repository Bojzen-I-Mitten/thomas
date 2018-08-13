#pragma warning(disable: 4717) // removes effect deprecation warning.

#include <ThomasCG.hlsl>

DepthStencilState DisableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ZERO;
};

RasterizerState TestRasterizerOutline
{
	FillMode = SOLID;
	CullMode = BACK;
	FrontCounterClockWise = TRUE;
	DepthClipEnable = TRUE;
};


struct v2f {
	float4 vertex : SV_POSITION;
	
};

v2f vert(appdata_thomas v)
{
	v2f o;
    o.vertex = ThomasObjectToClipPos(v.vertex + v.normal * 0.2f);
    

	return o;
}

float4 frag(v2f i) : SV_TARGET
{
	return float4(1,0.5f,0, 1.0f);
}


technique11 Standard {
	pass OutlinePass {
		VERT(vert());
		SetGeometryShader(NULL);
		FRAG(frag());
		SetDepthStencilState(DisableDepth, 0);
		SetRasterizerState(TestRasterizerOutline);
	}
}