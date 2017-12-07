cbuffer cameraBuffer : register(b0)
{
	float3 cameraUp;
	float deltaTime;
	float3 cameraRight;
	float pad;
};


struct ParticleStruct
{
	float3 position;
	float gravity;
	float3 direction;
	float speed;

	float endSpeed;
	float delay;
	float size;
	float endSize;

	float lifeTime;
	float lifeTimeLeft;
	float rotationSpeed;
	float rotation;

	float4 startColor;

	float4 endColor;

};

struct BillboardStruct
{
	float3 quad[2][3];
	float2 pad2;
	float2 uvs[2][3];
	float4 colorFactor;
};

StructuredBuffer<ParticleStruct> particlesRead : register(t0);
RWStructuredBuffer<ParticleStruct> particlesWrite : register(u6);
RWStructuredBuffer<BillboardStruct> billboards : register(u7);


[numthreads(256, 1, 1)]
void CSMain(uint3 Gid : SV_GroupID, uint3 GTid : SV_GroupThreadID)
{
	float index = (Gid.x * 256) + GTid.x;

    
	if (particlesRead[index].delay < 0.0f)
	{
        //ANIMATE
		
        
		particlesWrite[index].delay = particlesRead[index].delay;
		particlesWrite[index].speed = particlesRead[index].speed;
        
		particlesWrite[index].size = particlesRead[index].size;
        
		particlesWrite[index].lifeTimeLeft = particlesRead[index].lifeTimeLeft;
        
		float lerpValue = 1 - (particlesRead[index].lifeTimeLeft / particlesRead[index].lifeTime);

		float speed = lerp(particlesRead[index].speed, particlesRead[index].endSpeed, lerpValue);

		float3 particlePosWS = particlesRead[index].position + particlesRead[index].direction * speed * deltaTime + float3(0, 1, 0) * particlesRead[index].gravity * deltaTime;
		particlesWrite[index].position = particlePosWS;


		float scale = 0;

		if (particlesRead[index].lifeTimeLeft < 0.0f)
		{
			scale = 0;
			billboards[index].colorFactor = float4(0, 0, 0, 0);

		}
		else
		{
			scale = lerp(particlesRead[index].size, particlesRead[index].endSize, lerpValue);
			billboards[index].colorFactor = lerp(particlesRead[index].startColor, particlesRead[index].endColor, lerpValue);
			particlesWrite[index].lifeTimeLeft = particlesRead[index].lifeTimeLeft - deltaTime;
            
		}
        //BILLBOARD
		float3 right = cameraRight * scale;
		float3 up = -cameraUp * scale;

		particlesWrite[index].rotation = particlesRead[index].rotation + particlesRead[index].rotationSpeed * deltaTime;

		float sinangle = sin(particlesRead[index].rotation);
		float cosangle = cos(particlesRead[index].rotation);

		float3 temp = cosangle * right - sinangle * up;
		right = sinangle * right + cosangle * up;
		up = temp;

        //tri 1
		billboards[index].quad[0][0] = particlePosWS + up + right;
		billboards[index].quad[0][1] = particlePosWS + up - right;
		billboards[index].quad[0][2] = particlePosWS - up + right;
        billboards[index].uvs[0][0] = float2(1, 1);
        billboards[index].uvs[0][1] = float2(1, 0);
        billboards[index].uvs[0][2] = float2(0, 1);
        //tri 2
		billboards[index].quad[1][0] = particlePosWS - up + right;
		billboards[index].quad[1][1] = particlePosWS + up - right;
		billboards[index].quad[1][2] = particlePosWS - up - right;
        billboards[index].uvs[1][0] = float2(0, 1);
        billboards[index].uvs[1][1] = float2(1, 0);
        billboards[index].uvs[1][2] = float2(0, 0);

		
	}
	else
	{
        float3 right = 0.0f;
        //cameraRight * scale;
        float3 up = 0.0f;
        //-cameraUp * scale;
        //tri 1
        billboards[index].quad[0][0] = 0;
        billboards[index].quad[0][1] = 0;
        billboards[index].quad[0][2] = 0;
        billboards[index].uvs[0][0] = float2(1, 1);
        billboards[index].uvs[0][1] = float2(1, 0);
        billboards[index].uvs[0][2] = float2(0, 1);
        //tri 2
        billboards[index].quad[1][0] = 0;
        billboards[index].quad[1][1] = 0;
        billboards[index].quad[1][2] = 0;
        billboards[index].uvs[1][0] = float2(0, 1);
        billboards[index].uvs[1][1] = float2(1, 0);
        billboards[index].uvs[1][2] = float2(0, 0);
		particlesWrite[index].delay = particlesRead[index].delay - deltaTime;
	}
}
