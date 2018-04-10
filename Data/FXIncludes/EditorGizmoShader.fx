#pragma warning(disable: 4717) // removes effect deprecation warning.

#include <ThomasCG.fx>

PROPERTIES_START
float4 color;
PROPERTIES_END

DepthStencilState DisableDepth
{
	DepthEnable = FALSE;
	DepthWriteMask = ZERO;
	DepthFunc = NEVER;
	StencilEnable = FALSE;
};

RasterizerState TestRasterizerOutline
{
	FillMode = SOLID;
	CullMode = BACK;
	FrontCounterClockWise = TRUE;
	DepthClipEnable = FALSE;
};


struct appdata {
	float3 vertex: POSITION;
};

struct v2f {
	float4 vertex : SV_POSITION;
};

v2f vert(appdata_thomas v)
{
	v2f o;
	o.vertex = ThomasObjectToClipPos(v.vertex);

	return o;
}

float4 frag(v2f i) : SV_TARGET
{
	return color;
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