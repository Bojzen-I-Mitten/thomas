#pragma once
#include <vector>
#include "../utils/Math.h"

#define MAX_NUMBER_OF_POINTLIGHTS 32
#define MAX_NUMBER_OF_SPOTLIGHTS 32
#define MAX_NUMBER_OF_DIRECTIONALLIGHTS 32
#define MAX_NUMBER_OF_LIGHTS MAX_NUMBER_OF_DIRECTIONALLIGHTS + MAX_NUMBER_OF_POINTLIGHTS + MAX_NUMBER_OF_SPOTLIGHTS

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

		public:	
			Light();
			~Light() = default;
			static void Init();
			static void Destroy();
			static void Bind();
		
		public:
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
			LightType m_type;
			static int s_nrOfLights;
			static std::vector<Light> s_lights;
			static ID3D11Buffer* s_lightBuffer;
		};
	}
}