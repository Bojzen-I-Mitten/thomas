#pragma once
#include "..\utils\Math.h"

namespace thomas
{
	namespace graphics
	{
		class LightManager
		{
		public:
			struct DirectionalLightStruct
			{
				thomas::math::Vector4 lightColor;
				thomas::math::Vector3 lightDirection;
				float padding;
			};

			struct PointLightStruct
			{
				float constantAttenuation;
				float linearAttenuation;
				float quadraticAttenuation;
				float power;
				thomas::math::Vector4 lightColor;
				thomas::math::Vector3 position;
				float padding;		
			};

			struct LightBufferStruct
			{
				int nrOfDirectionalLights;
				int nrOfPointLights;
				int padding1;
				int padding2;
				DirectionalLightStruct directionalLights[3];
				PointLightStruct pointLights[20];
				
			} static s_lightstruct;

		public:	
			LightManager();
			~LightManager();
			static void Destroy();

		public:
			static int AddDirectionalLight(DirectionalLightStruct directionalLight);	
			static int AddPointLight(PointLightStruct pointLight);

		public:
			static bool UpdateDirectionalLight(DirectionalLightStruct other, int index);
			static bool UpdatePointLight(PointLightStruct other, int index);

		public:
			static bool BindDirectionalLight(unsigned int index);
			static bool BindPointLight(unsigned int index);

		private:
			static bool UpdateLightBuffer();
			
		private:
			static ID3D11Buffer* s_lightBuffer;
		};
	}
}