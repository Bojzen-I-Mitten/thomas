#pragma warning(disable: 4717) // removes effect deprecation warning.

#include "ThomasCG.fx"


cbuffer GizmoBuffer
{
	matrix gizmoMatrix;
	float4 gizmoColor;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
	DepthFunc = LESS_EQUAL;
};

RasterizerState SolidRasterizer
{
	FillMode = SOLID;
	CullMode = BACK;
	FrontCounterClockWise = TRUE;
	DepthClipEnable = FALSE;
};

RasterizerState WireframeRasterizer
{
	FillMode = WIREFRAME;
	CullMode = NONE;
	FrontCounterClockWise = TRUE;
	DepthClipEnable = FALSE;
};

struct v2f {
	float4 vertex : SV_POSITION;
};

v2f vert(appdata_thomas v)
{
	v2f o;
	o.vertex = mul(gizmoMatrix, v.vertex);
	o.vertex = ThomasWorldToClipPos(o.vertex);
	return o;
}

float4 frag(v2f i) : SV_TARGET
{
	return gizmoColor;
}


technique11 Standard {

	pass SOLID {
		VERT(vert());
		SetGeometryShader(NULL);
		FRAG(frag());
		SetDepthStencilState(EnableDepth, 0);
		SetRasterizerState(SolidRasterizer);
	}

	pass WIREFRAME {
		VERT(vert());
		SetGeometryShader(NULL);
		FRAG(frag());
		SetDepthStencilState(EnableDepth, 0);
		SetRasterizerState(WireframeRasterizer);
	}


}