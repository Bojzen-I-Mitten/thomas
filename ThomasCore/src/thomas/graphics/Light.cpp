#include "Light.h"
#include "../utils/d3d.h"

namespace thomas
{
	namespace graphics
	{
		Light::Light()
		{
			if (s_nrOfLights >= s_maxNrOfLights)
				delete this;

			
			m_data.attenuation = math::Vector3(1.0f, 0.2f, 0.0f);
			m_data.color = math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			m_data.direction = math::Vector4(0, 0, 0, 0);
			m_data.intensity = 1.0f;
			m_data.position = math::Vector4(0, 0, 0, 1);
			m_data.smoothness = 16.0f;
			m_data.spotInnerAngle = 0.0f;
			m_data.spotOuterAngle = 20.0f;
			m_data.type = (int)LightType::UNKNOWN;
			
			//thomas::utils::D3d::FillDynamicBufferVector(s_lightBuffer, s_lights);



			this->s_lights.push_back(*this);
			s_nrOfLights++;
		}

		Light::~Light()
		{

		}

		void Light::Init()
		{
			s_nrOfLights = 0;
			s_lights.reserve(sizeof(Light)*s_maxNrOfLights);
			//thomas::utils::D3d::CreateDynamicBufferFromVector(s_lights, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
		}

		void Light::Destroy()
		{
			
			s_lights.clear();
		}

		void Light::Bind()
		{

		}
	}
}