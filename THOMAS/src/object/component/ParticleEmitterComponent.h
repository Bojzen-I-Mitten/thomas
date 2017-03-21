#pragma once
#include "Component.h"
#include "..\..\utils\math.h"
#include "..\..\graphics\Texture.h"
#include "..\..\graphics\Shader.h"
#include "..\..\graphics\ParticleSystem.h"
#include "../../utils/DebugTools.h"

namespace thomas
{
	namespace object
	{
		namespace component
		{
			class THOMAS_API ParticleEmitterComponent : public Component
			{
			public:
				enum class BlendStates {
					ADDITIVE,
					ALPHA_BLEND
				};

				struct D3DData {
					ID3D11UnorderedAccessView* particleUAV1;
					ID3D11ShaderResourceView* particleSRV1;
					ID3D11Buffer* particleBuffer1;
					ID3D11UnorderedAccessView* particleUAV2;
					ID3D11ShaderResourceView* particleSRV2;
					ID3D11Buffer* particleBuffer2;
					ID3D11Buffer* particleBuffer;
					bool swapUAVandSRV;

					ID3D11UnorderedAccessView* billboardsUAV;
					ID3D11ShaderResourceView* billboardsSRV;
					ID3D11Buffer* billboardBuffer;
				};

				struct InitParticleBufferStruct
				{
					math::Vector3 position;
					float spread;
					
					unsigned int currentParticleStartIndex;
					float maxSpeed;
					float radius;
					bool spawnAtSphereEdge;

					float minSpeed;
					float endSpeed;
					float maxDelay;
					float minDelay;

					float maxSize;
					float minSize;
					float endSize;
					float maxLifeTime;

					float minLifeTime;
					float rand;
					float rotationSpeed;
					float rotation;

					math::Color startColor;

					math::Color endColor;
					math::Matrix directionMatrix;

					float gravity;
					math::Vector3 padding;

				};

				struct ParticleStruct
				{
					math::Vector3 position;
					float gravity;

					math::Vector3 direction;
					float speed;

					float endSpeed;
					float delay;
					float size;
					float endSize;

					float lifeTime;
					float lifeTimeLeft;
					float rotationSpeed;
					float rotation;
					

					math::Vector4 startColor;

					math::Vector4 endColor;
					
				};
			private:
				void CreateParticleUAVsandSRVs();
				void CreateInitBuffer();
				void CalculateMaxNrOfParticles();
			public:
				ParticleEmitterComponent();
				~ParticleEmitterComponent();

				void Start();
				void Update();

				void TogglePause();

				void SetSpread(float const other);
				void SetDirection(math::Vector3 other);
				void SetDirection(float const x, float const y, float const z);
				void SetSpeed(float const min, float const max);
				void SetSpeed(float const speed);
				void SetMaxSpeed(float const other);
				void SetMinSpeed(float const other);
				void SetEndSpeed(float const other);
				void SetDelay(float const min, float const max);
				void SetDelay(float const delay);
				void SetMaxDelay(float const other);
				void SetMinDelay(float const other);
				void SetSize(float const min, float const max);
				void SetSize(float const size);
				void SetMaxSize(float const other);
				void SetMinSize(float const other);
				void SetEndSize(float const other);
				void SetLifeTime(float const min, float const max);
				void SetLifeTime(float lifeTime);
				void SetMaxLifeTime(float const other);
				void SetMinLifeTime(float const other);
				void SetRotationSpeed(float const other);
				void SetRotation(float const other);
				void SetLooping(bool const other);

				void SetGravity(float const other);
				
				void SetStartColor(math::Vector4 const other);
				void SetEndColor(math::Vector4 const other);
				
				void SetRadius(float radius);
				void SpawnAtSphereEdge(bool other);
				

				void StartEmitting();
				void StopEmitting(bool force=false);
				
				bool IsEmitting() const;
				
				void SetOffset(math::Vector3 offset);
				void SetOffset(float x, float y, float z);
				
				void SetShader(std::string shaderName);
				graphics::Shader* GetShader();
				void SetTexture(std::string texturePath);
				graphics::Texture* GetTexture();

				void SetEmissionRate(float emissionRate);
				void SetEmissionDuration(float duration);
				float GetEmissionRate();
				unsigned int GetNrOfMaxParticles() const;

				D3DData* GetD3DData();

				void AddToDebugMenu();

				std::string GetDebugMenuName();

				InitParticleBufferStruct* GetInitData();

				bool IsPaused();

				void ExportEmitter(std::string path);
				void ImportEmitter(std::string path);

				void SetBlendState(BlendStates state);
				BlendStates GetBlendState();

				float GetSpawnedParticleCount();

				float GetDrawTimer();
			private:
				std::string m_debugBarName;
				math::Vector3 m_offset;
				math::Vector3 m_directionVector;
				D3DData m_d3dData;
				graphics::Shader* m_shader;
				graphics::Texture* m_texture;

				InitParticleBufferStruct m_particleBufferStruct;

				bool m_paused;
				float m_tempMaxDelay;
				float m_tempMaxLifeTime;
				float m_tempEmissionRate;

				bool m_isEmitting;
				bool m_looping;
				float m_emissionDuration;
				bool m_shouldUpdateResources;
				unsigned int m_maxNrOfParticles;
				unsigned int m_spawnedParticleCount;
				float m_emissionRate;
				float m_emissionTimer;
				float m_emissionTimeLeft;

				float m_drawTimer;

				BlendStates m_blendState;

			};
		}
	}
}
