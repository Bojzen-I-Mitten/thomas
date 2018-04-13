#pragma once
#include "../utils/Math.h"
#include <d3d11.h>


namespace thomas
{
	namespace object
	{
		namespace component
		{
			class ParticleEmitterComponent;
			class Transform;
			class Camera;
			
		}
	}
	namespace resource
	{
		class ComputeShader;
	}
	namespace graphics
	{
		
		class Mesh;
		class ParticleSystem
		{
		private:
			

			static void UpdateCameraBuffers(object::component::Transform* trans, math::Matrix viewProjMatrix, bool paused);
			static void SwapUAVsandSRVs(object::component::ParticleEmitterComponent * emitter);//ping pong
			
		public:
			ParticleSystem();
			~ParticleSystem();

			static void CreateBillboardUAVandSRV(int maxAmountOfParticles, ID3D11Buffer*& buffer, ID3D11UnorderedAccessView*& uav, ID3D11ShaderResourceView*& srv);
			static void Init();
			static void Destroy();
			static void SpawnParticles(object::component::ParticleEmitterComponent* emitter, int amountOfParticles);
			static void UpdateParticles(object::component::ParticleEmitterComponent* emitter);
			static void DrawParticles(object::component::Camera * camera, object::component::ParticleEmitterComponent* emitter);

			static ID3D11DepthStencilState* GetDepthStencilState();

		private:


			struct BlendStates {
				ID3D11BlendState* additive;
				ID3D11BlendState* alphaBlend;
			};

			static BlendStates s_blendStates;

			struct BillboardStruct
			{
				math::Vector3 positions[2][3];
				math::Vector2 pad2;
				math::Vector2 uvs[2][3];
				math::Vector4 colorFactor;
			};
			struct CameraBufferStruct
			{
				math::Vector3 up;
				float deltaTime;
				math::Vector3 right;
				float pad;
			};
			
			static CameraBufferStruct s_cameraBufferStruct;
			static math::Matrix s_viewProjMatrix;
			static ID3D11Buffer* s_cameraBuffer;
			
			static resource::ComputeShader* s_updateParticlesCS;
			static resource::ComputeShader* s_emitParticlesCS;

			static ID3D11UnorderedAccessView* s_activeParticleUAV;
			static ID3D11ShaderResourceView* s_activeParticleSRV;

			static Mesh* s_emptyMesh;
			
			static ID3D11DepthStencilState* s_depthStencilState;

			static unsigned int s_maxNumberOfBillboardsSupported;
		public:

		};

	
	}
}