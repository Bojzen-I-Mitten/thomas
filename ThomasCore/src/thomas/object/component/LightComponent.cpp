#include "LightComponent.h"
#include "../GameObject.h"
#include "../../editor/gizmos/Gizmos.h"

namespace thomas {
	namespace object {
		namespace component {
			
			std::vector<LightComponent*>	LightComponent::s_lightComponents;

			

			LightComponent::LightComponent()
			{
				attenuation = math::Vector3(1.0f, 0.2f, 0.0f);
				color = math::Color(1.0f, 1.0f, 1.0f);
				intensity = 1.0f;
				spotInnerAngle = 0.0f;
				spotOuterAngle = 20.0f;
				
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

			void LightComponent::OnDrawGizmos()
			{
				/*
				 editor::Gizmos::SetMatrix(math::Matrix::CreateWorld(m_bounds.Center, math::Vector3::Forward, math::Vector3::Up));
				 editor::Gizmos::SetColor(math::Color(0, 0, 1));
				 editor::Gizmos::DrawBoundingOrientedBox(m_bounds);*/
			}
			

			void LightComponent::SetColor(math::Color color)
			{
				color = color;
			}
			math::Color LightComponent::GetColor()
			{
				return color;
			}
			void LightComponent::SetIntensity(float intensity)
			{
				intensity = intensity;
			}
			float LightComponent::GetIntensity()
			{
				return intensity;
			}
			void LightComponent::SetSpotInnerAngle(float angle)
			{
				spotInnerAngle = angle;
			}
			float LightComponent::GetSpotInnerAngle()
			{
				return spotInnerAngle;
			}
			void LightComponent::SetSpotOuterAngle(float angle)
			{
				spotOuterAngle = angle;
			}
			float LightComponent::GetSpotOuterAngle()
			{
				return spotOuterAngle;
			}
			void LightComponent::SetConstantAttenuation(float x)
			{
				attenuation.x = x;
			}
			float LightComponent::GetConstantAttenuation()
			{
				return attenuation.x;
			}
			void LightComponent::SetLinearAttenuation(float y)
			{
				attenuation.y = y;
			}
			float LightComponent::GetLinearAttenuation()
			{
				return attenuation.y;
			}
			void LightComponent::SetQuadraticAttenuation(float z)
			{
				attenuation.z = z;
			}
			float LightComponent::GetQuadraticAttenuation()
			{
				return attenuation.y;
			}
			math::Vector3 LightComponent::GetAttenuation()
			{
				return attenuation;
			}

			void LightComponent::SetLightType(LIGHT_TYPES type)
			{
				m_type = type;
			}

			LightComponent::LIGHT_TYPES LightComponent::GetType()
			{
				return m_type;
			}
		}
	}
}