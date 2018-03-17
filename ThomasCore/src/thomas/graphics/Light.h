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
				math::Vector4			position;
				math::Vector4			direction;

				int						type;
			} m_data;
			
		private:
			LightType						m_type;

			static const int				s_maxNrOfLights = 2;
			static int						s_nrOfLights;
			static std::vector<Light>		s_lights;
			static std::vector<Light*>		s_pointers;
			static ID3D11Buffer*			s_lightBuffer;
		};

	}
}