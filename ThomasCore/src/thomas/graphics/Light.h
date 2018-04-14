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
				math::Vector3	color;
				float			intensity;
				math::Vector3	position;
				float			spotOuterAngle;
				math::Vector3	direction;
				float			spotInnerAngle;
				math::Vector3	attenuation;
				float   pad;
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