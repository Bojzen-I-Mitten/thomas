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

			m_data.direction = math::Vector4(0, 0, 0, 0);
			m_data.position = math::Vector4(0, 0, 0, 1);
			m_data.type = (int)LightType::UNKNOWN;

			thomas::utils::D3d::FillDynamicBufferVector(s_lightBuffer, s_lights);

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
			s_pointers.reserve(sizeof(Light*)*s_maxNrOfLights);
			thomas::utils::D3d::CreateDynamicBufferFromVector(s_lights, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
		}

		void Light::Destroy()
		{
			for (Light* p : s_pointers)
			{
				delete p;
			}
			s_pointers.clear();
			s_lights.clear();
		}

		void Light::Bind()
		{

		}
	}
}