#ifndef THOMAS_SHADER_VARIABLES_INCLUDED
#define THOMAS_SHADER_VARIABLES_INCLUDED

#define THOMAS_MATRIX_P glstate_matrix_projection
#define THOMAS_MATRIX_V thomas_MatrixV
#define THOMAS_MATRIX_I_V thomas_MatrixInvV
#define THOMAS_MATRIX_VP thomas_MatrixVP
#define THOMAS_MATRIX_M thomas_ObjectToWorld

#define THOMAS_MATRIX_MVP mul(thomas_MatrixVP, thomas_ObjectToWorld)
#define THOMAS_MATRIX_MV mul(thomas_MatrixV, thomas_ObjectToWorld)
#define THOMAS_MATRIX_T_MV transpose(THOMAS_MATRIX_MV)
#define THOMAS_MATRIX_IT_MV transpose(mul(thomas_WorldToObject, thomas_MatrixInvV))

#define THOMAS_LIGHTMODEL_AMBIENT (glstate_lightmodel_ambient * 2)

// ----------------------------------------------------------------------------

cbuffer ThomasPerCamera
{
	// Time (t = time since current level load) values from Thomas
	float4 _Time; // (t/20, t, t*2, t*3)
	float4 _SinTime; // sin(t/8), sin(t/4), sin(t/2), sin(t)
	float4 _CosTime; // cos(t/8), cos(t/4), cos(t/2), cos(t)
	float4 thomas_DeltaTime; // dt, 1/dt, smoothdt, 1/smoothdt
	float3 _WorldSpaceCameraPos;

	// x = 1 or -1 (-1 if projection is flipped)
	// y = near plane
	// z = far plane
	// w = 1/far plane
	float4 _ProjectionParams;

	// x = width
	// y = height
	// z = 1 + 1.0/width
	// w = 1 + 1.0/height
	float4 _ScreenParams;

	// Values used to linearize the Z buffer (http://www.humus.name/temp/Linearize%20depth.txt)
	// x = 1-far/near
	// y = far/near
	// z = x/far
	// w = y/far
	// or in case of a reversed depth buffer (THOMAS_REVERSED_Z is 1)
	// x = -1+far/near
	// y = 1
	// z = x/far
	// w = 1/far
	float4 _ZBufferParams;

	// x = orthographic camera's width
	// y = orthographic camera's height
	// z = unused
	// w = 1.0 if camera is ortho, 0.0 if perspective
	float4 thomas_OrthoParams;
};
    


cbuffer ThomasPerCameraRare
{
	float4 thomas_CameraWorldClipPlanes[6];

	// Projection matrices of the camera. Note that this might be different from projection matrix
	// that is set right now, e.g. while rendering shadows the matrices below are still the projection
	// of original camera.
	float4x4 thomas_CameraProjection;
	float4x4 thomas_CameraInvProjection;
	float4x4 thomas_WorldToCamera;
	float4x4 thomas_CameraToWorld;
};




// ----------------------------------------------------------------------------

cbuffer ThomasPerDraw
{
	float4x4 thomas_ObjectToWorld;
	float4x4 thomas_WorldToObject;
	float4 thomas_LODFade; // x is the fade value ranging within [0,1]. y is x quantized into 16 levels
	float4 thomas_WorldTransformParams; // w is usually 1.0, or -1.0 for odd-negative scale transforms
};

cbuffer ThomasPerDrawRare
{
	float4x4 glstate_matrix_transpose_modelview0;
};


// ----------------------------------------------------------------------------

cbuffer ThomasPerFrame
{
	half4 glstate_lightmodel_ambient;
	half4 thomas_AmbientSky;
	half4 thomas_AmbientEquator;
	half4 thomas_AmbientGround;
	half4 thomas_IndirectSpecColor;

	float4x4 glstate_matrix_projection;
	float4x4 thomas_MatrixV;
	float4x4 thomas_MatrixInvV;
	float4x4 thomas_MatrixVP;

	half4 thomas_ShadowColor;
};