#pragma warning(disable: 4717) // removes effect deprecation warning.

#include "ThomasCG.fx"


DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
	DepthFunc = LESS_EQUAL;
};

RasterizerState TestRasterizer
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

float3 GetReflectVec(float3 inVec, float3 normal)
{
	return 2 * dot(normal, inVec)*normal - inVec;
}

float4 GetReflectVec(float4 inVec, float4 normal)
{
	return 2 * dot(normal, inVec)*normal - inVec;
}

float4 GetHalfwayVec(float4 lightDir, float4 viewDir)
{
    return normalize(viewDir + lightDir);
}

struct v2f {
	float4 vertex : SV_POSITION;
	float4 worldPos : POSITIONWS;
	float4 normal : NORMAL;
};

v2f vert(appdata_thomas v)
{
	v2f o;
	o.vertex = ThomasObjectToClipPos(v.vertex);
	o.worldPos = ThomasObjectToWorldPos(v.vertex);
	o.normal = float4(ThomasWorldToObjectDir(v.normal), 0);
	return o;
}

struct Light
{
    float4  color;
    float4  position;
    float4  direction;
    float   intensity;
    float   smoothness; //specularIntensity
    float   spotInnerAngle;
    float   spotOuterAngle;
    float3  attenuation;
    uint    type;
    //bool drawHalo;
};


float4 frag(v2f input) : SV_TARGET
{

    Light tempLight;
    tempLight.color = float4(0.5f, 0.5f, 0.1f, 1.0f);
    tempLight.position = float4(3, 3, 3, 1);
    tempLight.intensity = 1;
    tempLight.direction = normalize(float4(1, 1, 1, 0));
    tempLight.type = 2;
    tempLight.smoothness = 16.0f;
    tempLight.spotInnerAngle = 10.0f;
    tempLight.spotOuterAngle = 30.0f;
    tempLight.attenuation = float3(0.4f, 0.02f, 0.1f);
    
    float4 finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float4 ambient = float4(0.1f, 0.1f, 0.1f, 1.0f);
    float4 diffuse = float4(0.4f, 0.4f, 0.4f, 1.0f);
    float4 specular = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    float4 viewDir = normalize(float4(_WorldSpaceCameraPos - input.worldPos.xyz, 0.0f));
	float4 lightDir = float4(0, 0, 0, 0);
    float lightMultiplyer = 1.0f;

    if (0 == tempLight.type)//directional
    {
        lightDir = -tempLight.direction;
        lightMultiplyer = tempLight.color * 2.2f;
    }
    else if (1 == tempLight.type)//point
    {
        lightDir = tempLight.position - input.worldPos;
        float lightDistance = length(lightDir);
        lightDir = normalize(lightDir);

        lightMultiplyer = tempLight.color * tempLight.intensity / (tempLight.attenuation.x + tempLight.attenuation.y * lightDistance + tempLight.attenuation.z * lightDistance * lightDistance);
    }
    else if (2 == tempLight.type)//spot
    {
        lightDir = tempLight.position - input.worldPos;
        float lightDistance = length(lightDir);
        lightDir = normalize(lightDir);

        float angle = degrees(acos(dot(tempLight.direction, lightDir)));
        float spotFactor = 0.0f;
        if (angle < tempLight.spotInnerAngle)
        {
            spotFactor = 1.0f;
        }
        else if (angle < tempLight.spotOuterAngle)
        {
            spotFactor = 1.0f - smoothstep(tempLight.spotInnerAngle, tempLight.spotOuterAngle, angle);
        }

        lightMultiplyer = spotFactor * tempLight.color * tempLight.intensity / (tempLight.attenuation.x + tempLight.attenuation.y * lightDistance + tempLight.attenuation.z * lightDistance * lightDistance);
    }
    
	
    float lambertian = saturate(dot(input.normal, lightDir));
    float specularIntensity = 0.0f;
    if (lambertian > 0.0f)
    {
        specularIntensity = pow(saturate(dot(input.normal, GetHalfwayVec(viewDir, lightDir))), tempLight.smoothness); //blinn-phong
    }
    
    return saturate(ambient + (diffuse * lambertian + specular * specularIntensity) * lightMultiplyer);
}


technique11 Standard {
	pass P0 {
		VERT(vert());
		SetGeometryShader(NULL);
		FRAG(frag());
		SetDepthStencilState(EnableDepth, 0);
		SetRasterizerState(TestRasterizer);
		SetBlendState(AlphaBlendingOn, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}

}