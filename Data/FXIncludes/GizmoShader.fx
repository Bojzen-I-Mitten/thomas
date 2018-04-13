#pragma warning(disable: 4717) // removes effect deprecation warning.

#include <ThomasCG.fx>


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
		SetBlendState(AlphaBlendingOn, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}

	pass WIREFRAME {
		VERT(vert());
		SetGeometryShader(NULL);
		FRAG(frag());
		SetDepthStencilState(EnableDepth, 0);
		SetRasterizerState(WireframeRasterizer);
		SetBlendState(AlphaBlendingOn, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}


}