#pragma warning(disable: 4717) // removes effect deprecation warning.

#include <ThomasCG.hlsl>


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
	CullMode = NONE;
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

v2f billVert(appdata_thomas v)
{
	v2f o;
	//o.vertex = mul(gizmoMatrix, v.vertex);
	matrix mv = mul(thomas_MatrixV, gizmoMatrix);
	mv._m00 = 1.0f;
	mv._m10 = 0.0f;
	mv._m20 = 0.0f;

	// Second colunm.
	mv._m01 = 0.0f;
	mv._m11 = 1.0f;
	mv._m21 = 0.0f;

	// Thrid colunm.
	mv._m02 = 0.0f;
	mv._m12 = 0.0f;
	mv._m22 = 1.0f;
	o.vertex = mul(THOMAS_MATRIX_P, mul(mv, v.vertex));
	//o.vertex = ThomasWorldToClipPos(o.vertex);
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

	pass BILLBOARD {
		VERT(billVert());
		SetGeometryShader(NULL);
		FRAG(frag());
		SetDepthStencilState(EnableDepth, 0);
		SetRasterizerState(SolidRasterizer);
		SetBlendState(AlphaBlendingOn, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}

}