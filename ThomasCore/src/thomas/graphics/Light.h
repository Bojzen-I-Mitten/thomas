#pragma once
#include <vector>
#include "../utils/Math.h"

namespace thomas
{
	namespace graphics
	{
		class Light
		{
		public:
			enum class LightType
			{
				DIRECTIONAL		= 1,
				POINT			= 2,
				SPOT			= 4,

				UNKNOWN			= 0
			};

			
			Light();
			~Light();

			static void Init();
			static void Destroy();
			static void Bind();

			struct LightStruct
			{
				thomas::math::Vector4  color;
				thomas::math::Vector4 position;
				thomas::math::Vector4 direction;
				float   intensity;
				float   smoothness; 
				float   spotInnerAngle;
				float   spotOuterAngle;
				thomas::math::Vector3 attenuation;
				unsigned int    type;
			} m_data;
			
		private:
			LightType						m_type;

			static const int				s_maxNrOfPointLights = 32;
			static const int				s_maxNrOfSpotLights = 32;
			static const int				s_maxNrOfDirectionalLights = 5;
			static const int				s_maxNrOfLights = s_maxNrOfDirectionalLights + s_maxNrOfPointLights + s_maxNrOfSpotLights;

			static int						s_nrOfLights;
			static std::vector<Light>		s_lights;
			static ID3D11Buffer*			s_lightBuffer;
		};

	}
}