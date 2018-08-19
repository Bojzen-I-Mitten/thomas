#include "LightComponent.h"
#include "../../utils/d3d.h"
#include "../GameObject.h"
#include "Transform.h"

namespace thomas
{
	namespace object
	{
		namespace component
		{

			LightComponent::LightComponent()
			{
			}
			LightComponent::~LightComponent()
			{
			}
			
			/**
			DIRECTIONAL LIGHT
			The light look down its forward transformation
			*/
			DirectionalLight::DirectionalLight() : LightComponent()
			{
				//standard init of light
				
				m_thisLight.lightColor = thomas::math::Vector4(1, 1, 1, 1);

				m_index = graphics::LightManager::AddDirectionalLight(m_thisLight);

				/*if ((m_index = graphics::LightManager::AddDirectionalLight(m_thisLight)) < 0)
				{
					LOG("Cant add light")
					Destroy(this);
				}*/
				
				
			}

			DirectionalLight::~DirectionalLight()
			{
			}

			bool DirectionalLight::Bind()
			{
				return true;
			}

			bool DirectionalLight::SetLightColor(thomas::math::Vector4 other)
			{
				m_thisLight.lightColor = other;
				return thomas::graphics::LightManager::UpdateDirectionalLight(m_thisLight, m_index);
				
			}
			
			
			void DirectionalLight::Update()
			{
				
				m_thisLight.lightDirection = m_gameObject->m_transform->Forward();
				thomas::graphics::LightManager::UpdateDirectionalLight(m_thisLight, m_index);
				return;
			}
			

			/**
			POINT LIGHT
			*/
			PointLight::PointLight() : LightComponent()
			{
				//standard init of light
				m_thisLight.constantAttenuation = 0.6f;
				m_thisLight.linearAttenuation = 0.3f;
				m_thisLight.quadraticAttenuation = 0.1f;
				m_thisLight.power = 5;
				
				m_thisLight.position = thomas::math::Vector3(0, 0, 0);
				m_thisLight.lightColor = thomas::math::Vector4(1, 1, 1, 1);

				m_index = graphics::LightManager::AddPointLight(m_thisLight);
			}
			PointLight::~PointLight()
			{
			}

			bool PointLight::Bind()
			{
				return true;
			}

			bool PointLight::SetLightColor(thomas::math::Vector4 other)
			{
				m_thisLight.lightColor = other;
				return thomas::graphics::LightManager::UpdatePointLight(m_thisLight, m_index);
			}
			

			bool PointLight::SetConstantAttenuation(float other)
			{
				m_thisLight.constantAttenuation = other;
				return thomas::graphics::LightManager::UpdatePointLight(m_thisLight, m_index);
			}
			bool PointLight::SetLinearAttenuation(float other)
			{
				m_thisLight.constantAttenuation = other;
				return thomas::graphics::LightManager::UpdatePointLight(m_thisLight, m_index);
			}
			bool PointLight::SetQuadraticAttenuation(float other)
			{
				m_thisLight.constantAttenuation = other;
				return thomas::graphics::LightManager::UpdatePointLight(m_thisLight, m_index);
			}

			
			bool PointLight::SetPower(float other)
			{
				m_thisLight.power = other;
				return thomas::graphics::LightManager::UpdatePointLight(m_thisLight, m_index);
			}
			void PointLight::Update()
			{
				m_thisLight.position = m_gameObject->m_transform->GetPosition();
				
				thomas::graphics::LightManager::UpdatePointLight(m_thisLight, m_index);
				return;
			}
		}
	}
}