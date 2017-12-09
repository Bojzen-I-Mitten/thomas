#pragma warning(disable: 4717) // removes effect deprecation warning.

#include "ThomasCG.fx"

DepthStencilState DisableDepth
{
	DepthEnable = FALSE;
	DepthWriteMask = ZERO;
	DepthFunc = LESS_EQUAL;
};

RasterizerState TestRasterizerOutline
{
	FillMode = SOLID;
	CullMode = FRONT;
	FrontCounterClockWise = TRUE;
	DepthClipEnable = FALSE;
};


struct v2f {
	float4 vertex : SV_POSITION;
};

v2f vert(appdata_thomas v)
{
	v2f o;
	o.vertex = ThomasObjectToClipPos(v.vertex);
	float4 normal = ThomasObjectToClipPos(v.normal);
	o.vertex += normal * 0.03f;
	return o;
}

float4 frag(v2f i) : SV_TARGET
{
	return float4(1,1,0, 1.0f);
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