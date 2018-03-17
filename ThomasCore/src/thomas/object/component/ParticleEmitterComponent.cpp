#include "ParticleEmitterComponent.h"
#include <cstdlib>
#include <ctime>
#include "../GameObject.h"
#include <fstream>
#include "../../utils/DebugTools.h"
#include "../../utils/d3d.h"

#include "..\..\graphics\Material.h"
#include "..\..\graphics\ParticleSystem.h"
#include "../../utils/DebugTools.h"
#include "Transform.h"
#include "../../ThomasTime.h"
namespace thomas
{
	namespace object
	{
		namespace component
		{
			thomas::object::component::ParticleEmitterComponent::ParticleEmitterComponent()
			{
				m_blendState = BlendStates::ALPHA_BLEND;
				m_offset = math::Vector3(0, 0, 0);
				m_shouldUpdateResources = true;
				m_emissionDuration = 1.0;
				m_emissionTimeLeft = m_emissionDuration;
				m_emissionTimer = 0;
				m_emissionRate = 1.0;
				m_tempEmissionRate = 1.0;
				m_looping = false;
				m_maxNrOfParticles = 0;//256 * 100 + 254;
				m_isEmitting = false;
				m_particleBufferStruct.position = math::Vector3(0, 0, 0);
				m_particleBufferStruct.spread = 1.0f;
				m_directionVector = math::Vector3(1, 0, 0);
				m_particleBufferStruct.directionMatrix = math::Matrix::CreateLookAt(math::Vector3(0, 0, 0), math::Vector3(1, 0, 0), math::Vector3::Up).Transpose();
				m_particleBufferStruct.maxSpeed = 0.0f;
				m_particleBufferStruct.minSpeed = 0.0f;
				m_particleBufferStruct.endSpeed = 0.0f;
				m_particleBufferStruct.maxDelay = 0.0f;
				m_tempMaxDelay = 0.0f;
				m_particleBufferStruct.minDelay = 0.0f;
				m_particleBufferStruct.maxSize = 1.0f;
				m_particleBufferStruct.minSize = 1.0f;
				m_particleBufferStruct.endSize = 1.0f;
				m_particleBufferStruct.maxLifeTime = 1.0f;
				m_particleBufferStruct.minLifeTime = 1.0f;
				m_tempMaxLifeTime = 1.0f;
				m_particleBufferStruct.rotationSpeed = 0.0f;
				m_particleBufferStruct.rotation = 0;
				m_particleBufferStruct.startColor = math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
				m_particleBufferStruct.endColor = math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
				m_particleBufferStruct.currentParticleStartIndex = 0;
				m_particleBufferStruct.spawnAtSphereEdge = false;
				m_particleBufferStruct.radius = 0;
				m_particleBufferStruct.rand = (std::rand() % 1000) / 1000.f;
				m_particleBufferStruct.gravity = 0.0f;
				m_paused = false;
				m_d3dData.swapUAVandSRV = true;

				m_spawnedParticleCount = 0;
				CalculateMaxNrOfParticles();
				CreateInitBuffer();
			}

			ParticleEmitterComponent::~ParticleEmitterComponent()
			{
				
				SAFE_RELEASE(m_d3dData.particleBuffer1);
				SAFE_RELEASE(m_d3dData.particleBuffer2);
				SAFE_RELEASE(m_d3dData.particleUAV1);
				SAFE_RELEASE(m_d3dData.particleUAV2);
				SAFE_RELEASE(m_d3dData.particleSRV1);
				SAFE_RELEASE(m_d3dData.particleSRV2);
				SAFE_RELEASE(m_d3dData.billboardBuffer);
				SAFE_RELEASE(m_d3dData.billboardsSRV);
				SAFE_RELEASE(m_d3dData.billboardsUAV);
				if(!m_debugBarName.empty())
					utils::DebugTools::RemoveBar(m_debugBarName);
			}

			void ParticleEmitterComponent::Update()
			{
				if (m_particleBufferStruct.maxLifeTime != m_tempMaxLifeTime || m_particleBufferStruct.maxDelay != m_tempMaxDelay || m_emissionRate != m_tempEmissionRate)
					CalculateMaxNrOfParticles();

				if (m_emissionTimeLeft < 0.0f)
				{
					m_drawTimer -= ThomasTime::GetDeltaTime();
					StopEmitting();
				}


				if (m_shouldUpdateResources)
				{
					m_shouldUpdateResources = false;
					CreateParticleUAVsandSRVs();

					m_particleBufferStruct.currentParticleStartIndex = 0;
					ParticleEmitterComponent::InitParticleBufferStruct tempStruct = m_particleBufferStruct;
					tempStruct.startColor = math::Color(0, 0, 0, 0);
					tempStruct.endColor = math::Color(0, 0, 0, 0);
					tempStruct.minSize = 0;
					tempStruct.maxSize = 0;
					tempStruct.endSize = 0;
					utils::D3d::FillDynamicBufferStruct(m_d3dData.particleBuffer, tempStruct);
					graphics::ParticleSystem::SpawnParticles(this, m_maxNrOfParticles);
					m_spawnedParticleCount = m_maxNrOfParticles;
					graphics::ParticleSystem::UpdateParticles(this);
				}


				if (m_isEmitting && !m_paused)
				{
					if (!m_looping)
						m_emissionTimeLeft -= ThomasTime::GetDeltaTime();

					m_emissionTimer += ThomasTime::GetDeltaTime();
					UINT numberOfParticlesToEmit = m_emissionTimer / (1.0f / m_emissionRate);
					if (numberOfParticlesToEmit > 0)
					{
						m_emissionTimer = 0;
						m_particleBufferStruct.position = m_gameObject->m_transform->GetPosition() + math::Vector3::Transform(m_offset, math::Matrix::CreateFromQuaternion(m_gameObject->m_transform->GetRotation())) ;
						SetDirection(m_directionVector);
						m_particleBufferStruct.rand = (std::rand() % 1000) / 1000.f;
						utils::D3d::FillDynamicBufferStruct(m_d3dData.particleBuffer, m_particleBufferStruct);
						graphics::ParticleSystem::SpawnParticles(this, numberOfParticlesToEmit);
						m_particleBufferStruct.currentParticleStartIndex = (m_particleBufferStruct.currentParticleStartIndex + numberOfParticlesToEmit) % m_maxNrOfParticles;
						m_spawnedParticleCount += numberOfParticlesToEmit;
						m_spawnedParticleCount = min(m_spawnedParticleCount, m_maxNrOfParticles);
					}
				}
			}

			void ParticleEmitterComponent::TogglePause()
			{
				m_paused = !m_paused;
			}

			void ParticleEmitterComponent::SetSpread(float const other)
			{
				m_particleBufferStruct.spread = other;
			}
			void ParticleEmitterComponent::SetDirection(math::Vector3 other)
			{
				m_directionVector = other;
				math::Vector3 temp = math::Vector3::TransformNormal(other, m_gameObject->m_transform->GetWorldMatrix());
				//m_directionVector = other;
				other.x += 0.0000001;
				m_particleBufferStruct.directionMatrix = math::Matrix::CreateLookAt(math::Vector3(0, 0, 0), -temp, math::Vector3::Up).Invert().Transpose();
			}
			void ParticleEmitterComponent::SetDirection(float const x, float const y, float const z)
			{
				SetDirection(math::Vector3(x, y, z));
			}
			void ParticleEmitterComponent::SetSpeed(float const min, float const max)
			{
				SetMinSpeed(min);
				SetMaxSpeed(max);
			}
			void ParticleEmitterComponent::SetSpeed(float const speed)
			{
				SetSpeed(speed, speed);
				SetEndSpeed(speed);
			}
			void ParticleEmitterComponent::SetMaxSpeed(float const other)
			{
				m_particleBufferStruct.maxSpeed = other;
			}
			void ParticleEmitterComponent::SetMinSpeed(float const other)
			{
				m_particleBufferStruct.minSpeed = other;
			}
			void ParticleEmitterComponent::SetEndSpeed(float const other)
			{
				m_particleBufferStruct.endSpeed = other;
			}
			void ParticleEmitterComponent::SetDelay(float const min, float const max)
			{
				SetMinDelay(min);
				SetMaxDelay(max);
			}
			void ParticleEmitterComponent::SetDelay(float const delay)
			{
				SetDelay(delay, delay);

			}
			void ParticleEmitterComponent::SetMaxDelay(float const other)
			{
				m_particleBufferStruct.maxDelay = other;
				m_tempMaxDelay = other;
				CalculateMaxNrOfParticles();
			}
			void ParticleEmitterComponent::SetMinDelay(float const other)
			{
				m_particleBufferStruct.minDelay = other;
			}
			void ParticleEmitterComponent::SetSize(float const min, float const max)
			{
				SetMinSize(min);
				SetMaxSize(max);
			}
			void ParticleEmitterComponent::SetSize(float const size)
			{
				SetSize(size, size);
				SetEndSize(size);
			}
			void ParticleEmitterComponent::SetMaxSize(float const other)
			{
				m_particleBufferStruct.maxSize = other;
			}
			void ParticleEmitterComponent::SetMinSize(float const other)
			{
				m_particleBufferStruct.minSize = other;
			}
			void ParticleEmitterComponent::SetEndSize(float const other)
			{
				m_particleBufferStruct.endSize = other;
			}
			void ParticleEmitterComponent::SetLifeTime(float const min, float const max)
			{
				SetMinLifeTime(min);
				SetMaxLifeTime(max);
			}
			void ParticleEmitterComponent::SetLifeTime(float lifeTime)
			{
				SetLifeTime(lifeTime, lifeTime);

			}
			void ParticleEmitterComponent::SetMinLifeTime(float const other)
			{
				m_particleBufferStruct.minLifeTime = other;
			}
			void ParticleEmitterComponent::SetMaxLifeTime(float const other)
			{
				m_particleBufferStruct.maxLifeTime = other;
				m_tempMaxLifeTime = other;
				CalculateMaxNrOfParticles();
			}
			void ParticleEmitterComponent::SetRotationSpeed(float const other)
			{
				m_particleBufferStruct.rotationSpeed = other;
			}
			void ParticleEmitterComponent::SetRotation(float const other)
			{
				m_particleBufferStruct.rotation = other;
			}
			void ParticleEmitterComponent::SetLooping(bool const other)
			{
				m_looping = other;
			}
			void ParticleEmitterComponent::SetGravity(float const other)
			{
				m_particleBufferStruct.gravity = other;
			}
			void ParticleEmitterComponent::SetStartColor(math::Vector4 const other)
			{
				m_particleBufferStruct.startColor = other;
			}
			void ParticleEmitterComponent::SetEndColor(math::Vector4 const other)
			{
				m_particleBufferStruct.endColor = other;
			}

			void ParticleEmitterComponent::SetRadius(float radius)
			{
				m_particleBufferStruct.radius = radius;
			}

			void ParticleEmitterComponent::SpawnAtSphereEdge(bool other)
			{
				m_particleBufferStruct.spawnAtSphereEdge = other;
			}

			void ParticleEmitterComponent::StartEmitting()
			{
				m_drawTimer = m_particleBufferStruct.maxLifeTime + m_particleBufferStruct.maxDelay;
				if (!m_isEmitting)
				{
					m_isEmitting = true;
					m_emissionTimeLeft = m_emissionDuration;
				}
			}

			void ParticleEmitterComponent::StopEmitting(bool force)
			{
				m_isEmitting = false;
				if (force)
				{
					m_particleBufferStruct.currentParticleStartIndex = 0;
					ParticleEmitterComponent::InitParticleBufferStruct tempStruct = m_particleBufferStruct;
					tempStruct.minSize = 0;
					tempStruct.maxSize = 0;
					tempStruct.endSize = 0;
					utils::D3d::FillDynamicBufferStruct(m_d3dData.particleBuffer, tempStruct);
					graphics::ParticleSystem::SpawnParticles(this, m_maxNrOfParticles);

				}
			}

			bool ParticleEmitterComponent::IsEmitting() const
			{
				return m_isEmitting;
			}

			void ParticleEmitterComponent::SetOffset(math::Vector3 offset)
			{
				m_offset = offset;
			}

			void ParticleEmitterComponent::SetOffset(float x, float y, float z)
			{
				SetOffset(math::Vector3(x, y, z));
			}

			void ParticleEmitterComponent::SetMaterial(graphics::Material * material)
			{
				m_material = material;
			}

			graphics::Material * ParticleEmitterComponent::GetMaterial()
			{
				return m_material;
			}



			void ParticleEmitterComponent::CreateParticleUAVsandSRVs()
			{
				SAFE_RELEASE(m_d3dData.particleBuffer1);
				SAFE_RELEASE(m_d3dData.particleBuffer2);
				SAFE_RELEASE(m_d3dData.particleUAV1);
				SAFE_RELEASE(m_d3dData.particleUAV2);
				SAFE_RELEASE(m_d3dData.particleSRV1);
				SAFE_RELEASE(m_d3dData.particleSRV2);
				SAFE_RELEASE(m_d3dData.billboardBuffer);
				SAFE_RELEASE(m_d3dData.billboardsSRV);
				SAFE_RELEASE(m_d3dData.billboardsUAV);
				UINT bytewidth = sizeof(ParticleStruct) * m_maxNrOfParticles;
				UINT structurebytestride = sizeof(ParticleStruct);
				utils::D3d::CreateBufferAndUAV(NULL, bytewidth, structurebytestride, m_d3dData.particleBuffer1, m_d3dData.particleUAV1, m_d3dData.particleSRV1);
				utils::D3d::CreateBufferAndUAV(NULL, bytewidth, structurebytestride, m_d3dData.particleBuffer2, m_d3dData.particleUAV2, m_d3dData.particleSRV2);
				graphics::ParticleSystem::CreateBillboardUAVandSRV(m_maxNrOfParticles, m_d3dData.billboardBuffer, m_d3dData.billboardsUAV, m_d3dData.billboardsSRV);
			}

			void ParticleEmitterComponent::CreateInitBuffer()
			{
				m_d3dData.particleBuffer = thomas::utils::D3d::CreateDynamicBufferFromStruct(m_particleBufferStruct, D3D11_BIND_CONSTANT_BUFFER);
			}


			void ParticleEmitterComponent::CalculateMaxNrOfParticles()
			{
				m_particleBufferStruct.maxLifeTime = m_tempMaxLifeTime;
				m_particleBufferStruct.maxDelay = m_tempMaxDelay;
				m_emissionRate = m_tempEmissionRate;
				m_maxNrOfParticles = (m_particleBufferStruct.maxLifeTime + m_particleBufferStruct.maxDelay)*m_emissionRate;
				m_shouldUpdateResources = true;

			}

	
			void ParticleEmitterComponent::SetEmissionRate(float emissionRate)
			{
				m_emissionRate = emissionRate;
				m_tempEmissionRate = emissionRate;
				m_emissionTimer = 1.0f/m_emissionRate; //So we spawn one at least one at start :)
				CalculateMaxNrOfParticles();
			}

			void ParticleEmitterComponent::SetEmissionDuration(float duration)
			{
				m_emissionDuration = duration;
				m_emissionTimeLeft = m_emissionDuration;
			}

			float ParticleEmitterComponent::GetEmissionRate()
			{
				return m_emissionRate;
			}

			unsigned int ParticleEmitterComponent::GetNrOfMaxParticles() const
			{
				return m_maxNrOfParticles;
			}

			ParticleEmitterComponent::D3DData * ParticleEmitterComponent::GetD3DData()
			{
				return &m_d3dData;
			}

			void ParticleEmitterComponent::AddToDebugMenu()
			{
				m_debugBarName = "particleEmitter-" + std::to_string(rand());
				utils::DebugTools::CreateBar(m_debugBarName);
				std::string def = m_debugBarName + " size='400 600' valueswidth=fit refresh=0.001 iconifiable=false buttonalign=center";
				TwDefine(def.c_str());

				utils::DebugTools::AddBool(m_isEmitting, "Active", m_debugBarName);
				utils::DebugTools::AddFloat(m_emissionTimeLeft, "timeLeft", m_debugBarName);	
				utils::DebugTools::AddFloat(m_emissionDuration, "Duration", m_debugBarName);
				utils::DebugTools::AddBool(m_looping, "Looping", m_debugBarName);

				utils::DebugTools::AddDirectionVector(m_offset, "offset", m_debugBarName);
				utils::DebugTools::AddDirectionVector(m_directionVector, "direction", m_debugBarName);

				utils::DebugTools::AddFloat(m_particleBufferStruct.spread, "spread", m_debugBarName);
				utils::DebugTools::AddFloat(m_particleBufferStruct.gravity, "gravity", m_debugBarName);

				utils::DebugTools::AddFloat(m_particleBufferStruct.minSize, "minSize", m_debugBarName);
				utils::DebugTools::AddFloat(m_particleBufferStruct.maxSize, "maxSize", m_debugBarName);
				utils::DebugTools::AddFloat(m_particleBufferStruct.endSize, "endSize", m_debugBarName);

				utils::DebugTools::AddFloat(m_particleBufferStruct.minSpeed, "minSpeed", m_debugBarName);
				utils::DebugTools::AddFloat(m_particleBufferStruct.maxSpeed, "maxSpeed", m_debugBarName);
				utils::DebugTools::AddFloat(m_particleBufferStruct.endSpeed, "endSpeed", m_debugBarName);

				utils::DebugTools::AddFloat(m_particleBufferStruct.minDelay, "minDelay", m_debugBarName);
				utils::DebugTools::AddFloat(m_tempMaxDelay, "maxDelay", m_debugBarName);

				utils::DebugTools::AddFloat(m_particleBufferStruct.minLifeTime, "minLifeTime", m_debugBarName);
				utils::DebugTools::AddFloat(m_tempMaxLifeTime, "maxLifeTime", m_debugBarName);

				utils::DebugTools::AddColor(m_particleBufferStruct.startColor, "startColor", m_debugBarName);
				utils::DebugTools::AddColor(m_particleBufferStruct.endColor, "endColor", m_debugBarName);

				utils::DebugTools::AddFloat(m_particleBufferStruct.radius, "radius", m_debugBarName);

				utils::DebugTools::AddFloat(m_particleBufferStruct.rotationSpeed, "rotationSpeed", m_debugBarName);

				utils::DebugTools::AddBool(m_particleBufferStruct.spawnAtSphereEdge, "Spawn at edge", m_debugBarName);
				
				utils::DebugTools::AddFloat(m_tempEmissionRate, "Emission rate", m_debugBarName);
				
				TwEnumVal blendStates[] = { {(int)BlendStates::ADDITIVE, "Additive"}, {(int)BlendStates::ALPHA_BLEND, "Alpha Blend"} };
				TwType blendType = TwDefineEnum("Blend State", blendStates, 2);
				utils::DebugTools::AddEnum(blendType, *(int*)&m_blendState, "BlendState", m_debugBarName);
				
			}

			std::string ParticleEmitterComponent::GetDebugMenuName()
			{
				return m_debugBarName;
			}

			ParticleEmitterComponent::InitParticleBufferStruct * ParticleEmitterComponent::GetInitData()
			{
				return &m_particleBufferStruct;
			}

			bool ParticleEmitterComponent::IsPaused()
			{
				return m_paused;
			}

			void ParticleEmitterComponent::ExportEmitter(std::string path)
			{
				//Header size and texture size
				int texturesize;

				std::string matName = m_material->GetName();
				//Texturesize is how long the string is * bytes
				int matSize = matName.length() * sizeof(char);

				//Opens file
				std::ofstream file;
				file.open(path, std::ios::binary | std::ios::out);

				//Header
				file.write(reinterpret_cast<char*>(&matSize), sizeof(int));
				file.write(matName.c_str(), matSize);

				//Particle System variables
				file.write(reinterpret_cast<char*>(&m_offset), sizeof(math::Vector3));
				file.write(reinterpret_cast<char*>(&m_looping), sizeof(bool));
				file.write(reinterpret_cast<char*>(&m_directionVector), sizeof(math::Vector3));
				file.write(reinterpret_cast<char*>(&m_emissionDuration), sizeof(float));
				file.write(reinterpret_cast<char*>(&m_emissionRate), sizeof(float));
				file.write(reinterpret_cast<char*>(&m_blendState), sizeof(BlendStates));

				file.write(reinterpret_cast<char*>(&m_particleBufferStruct), sizeof(ParticleEmitterComponent::InitParticleBufferStruct));
				file.close();
			}

			void ParticleEmitterComponent::ImportEmitter(std::string path)
			{
				//Test reading file
				std::ifstream file;
				file.open(path, std::ios::binary | std::ios::in);

				int materialSize;
				file.read((char*)&materialSize, sizeof(int));

				//Read texture name
				char* materialName = (char*)malloc(materialSize + 1);
				file.read(materialName, sizeof(char) * materialSize);
				materialName[materialSize] = 0;

				//read data
				file.read((char*)&m_offset, sizeof(math::Vector3));
				file.read((char*)&m_looping, sizeof(bool));
				file.read((char*)&m_directionVector, sizeof(math::Vector3));
				file.read((char*)&m_emissionDuration, sizeof(float));
				file.read((char*)&m_emissionRate, sizeof(float));
				file.read((char*)&m_blendState, sizeof(BlendStates));
				

				//Read Particle struct
				file.read((char*)&m_particleBufferStruct, sizeof(ParticleEmitterComponent::InitParticleBufferStruct));
				//file.read((char*)&variable, sizeof(vartype));
				file.close();

				m_particleBufferStruct.currentParticleStartIndex = 0;


				m_tempEmissionRate = m_emissionRate;
				m_particleBufferStruct.maxDelay = m_tempMaxDelay;
				m_particleBufferStruct.maxLifeTime = m_tempMaxLifeTime;
				

				CalculateMaxNrOfParticles();

				SetMaterial(graphics::Material::Find(materialName));
			}

			unsigned int ParticleEmitterComponent::GetSpawnedParticleCount()
			{
				return m_spawnedParticleCount;
			}

			void ParticleEmitterComponent::SetBlendState(BlendStates state)
			{
				m_blendState = state;
			}

			ParticleEmitterComponent::BlendStates ParticleEmitterComponent::GetBlendState()
			{
				return m_blendState;
			}

			float ParticleEmitterComponent::GetDrawTimer()
			{
				return m_drawTimer;
			}


		}
	}
}