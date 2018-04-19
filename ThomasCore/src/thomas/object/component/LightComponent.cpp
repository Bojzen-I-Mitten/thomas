#include "LightComponent.h"
#include "../GameObject.h"
#include "../../editor/gizmos/Gizmos.h"

namespace thomas {
	namespace object {
		namespace component {

			std::vector<LightComponent::Light> LightComponent::s_dataVector;
			thomas::utils::buffers::StructuredBuffer* LightComponent::s_dataBuffer;
			std::vector<LightComponent*>	LightComponent::s_lightComponents;

			LightComponent::LightComponent()
			{
				m_attenuation = math::Vector3(1.0f, 0.2f, 0.0f);
				m_color = math::Color(1.0f, 1.0f, 1.0f);
				m_intensity = 1.0f;
				m_spotInnerAngle = 0.0f;
				m_spotOuterAngle = 20.0f;
				
				m_type = LIGHT_TYPES::DIRECTIONAL;

				s_lightComponents.push_back(this);
			}

			LightComponent::~LightComponent()
			{

			}


			void LightComponent::Update()
			{
				
			}


			void LightComponent::SubmitToRenderer(Camera* camera)
			{
				

			}

			void LightComponent::OnDestroy()
			{
				for (int i = 0; i < s_lightComponents.size(); i++)
				{
					if (s_lightComponents[i] == this)
					{
						s_lightComponents.erase(s_lightComponents.begin() + i);
						return;
					}
				}
			}

			std::vector<LightComponent*> LightComponent::GetAllLightComponents()
			{
				return s_lightComponents;
			}

			void LightComponent::UpdateLights()
			{
				s_dataVector.clear();
				for (LightComponent* lc : s_lightComponents)
				{
					Light temp;
					temp.color = lc->GetColor().ToVector3();
					temp.intensity = lc->GetIntensity();
					temp.position = lc->m_gameObject->m_transform->GetPosition();
					temp.spotOuterAngle = lc->GetSpotOuterAngle();
					temp.direction = lc->m_gameObject->m_transform->Forward();
					temp.spotInnerAngle = lc->GetSpotInnerAngle();
					temp.attenuation = lc->GetAttenuation();
					temp.type = (unsigned int)lc->GetType();

					s_dataVector.push_back(temp);
				}
				
				s_dataBuffer->SetData(s_dataVector.data(), s_dataVector.size() * sizeof(Light));
			}

			void LightComponent::OnDrawGizmos()
			{
				/*
				 editor::Gizmos::SetMatrix(math::Matrix::CreateWorld(m_bounds.Center, math::Vector3::Forward, math::Vector3::Up));
				 editor::Gizmos::SetColor(math::Color(0, 0, 1));
				 editor::Gizmos::DrawBoundingOrientedBox(m_bounds);*/
			}
			

			void LightComponent::SetColor(math::Color color)
			{
				m_color = color;
			}
			math::Color LightComponent::GetColor()
			{
				return m_color;
			}
			void LightComponent::SetIntensity(float intensity)
			{
				m_intensity = intensity;
			}
			float LightComponent::GetIntensity()
			{
				return m_intensity;
			}
			void LightComponent::SetSpotInnerAngle(float angle)
			{
				m_spotInnerAngle = angle;
			}
			float LightComponent::GetSpotInnerAngle()
			{
				return m_spotInnerAngle;
			}
			void LightComponent::SetSpotOuterAngle(float angle)
			{
				m_spotOuterAngle = angle;
			}
			float LightComponent::GetSpotOuterAngle()
			{
				return m_spotOuterAngle;
			}
			void LightComponent::SetConstantAttenuation(float x)
			{
				m_attenuation.x = x;
			}
			float LightComponent::GetConstantAttenuation()
			{
				return m_attenuation.x;
			}
			void LightComponent::SetLinearAttenuation(float y)
			{
				m_attenuation.y = y;
			}
			float LightComponent::GetLinearAttenuation()
			{
				return m_attenuation.y;
			}
			void LightComponent::SetQuadraticAttenuation(float z)
			{
				m_attenuation.z = z;
			}
			float LightComponent::GetQuadraticAttenuation()
			{
				return m_attenuation.y;
			}
			math::Vector3 LightComponent::GetAttenuation()
			{
				return m_attenuation;
			}

			void LightComponent::SetLightType(LIGHT_TYPES type)
			{
				m_type = type;
			}

			LightComponent::LIGHT_TYPES LightComponent::GetType()
			{
				return m_type;
			}

			void LightComponent::Init()
			{
				s_dataBuffer = new thomas::utils::buffers::StructuredBuffer(nullptr, sizeof(Light), NR_OF_LIGHTS, D3D11_USAGE_DYNAMIC);
			}
			void LightComponent::Destroy()
			{
				delete s_dataBuffer;
			}
		}
	}
}