#include "Light.h"

namespace thomas
{
	namespace graphics
	{
		Light::Light()
		{
			if (s_nrOfLights >= MAX_NUMBER_OF_LIGHTS)
				delete this;
	
			m_data.attenuation = math::Vector3(1.0f, 0.2f, 0.0f);
			m_data.color = math::Vector3(1.f);
			m_data.direction = math::Vector3();
			m_data.intensity = 1.0f;
			m_data.position = math::Vector3();
			m_data.spotInnerAngle = 0.f;
			m_data.spotOuterAngle = 20.f;
			
			s_lights.push_back(*this);
			s_nrOfLights++;
		}

		void Light::Init()
		{
			s_nrOfLights = 0;
			s_lights.reserve(sizeof(Light) * MAX_NUMBER_OF_LIGHTS);
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