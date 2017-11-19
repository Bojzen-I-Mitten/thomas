#pragma once
#include "../utils/Math.h"


namespace thomas
{
	namespace graphics
	{

		class LightManager
		{
		private:
			static bool UpdateLightBuffer();
		public:
			//make struct for colors?
			//struct....


			//match these structs on the graphicscard
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

			

			
			LightManager();
			~LightManager();
			static int AddDirectionalLight(DirectionalLightStruct directionalLight);
			
			static int AddPointLight(PointLightStruct pointLight);

			static bool UpdateDirectionalLight(DirectionalLightStruct other, int index);
			static bool UpdatePointLight(PointLightStruct other, int index);
			

			static bool BindAllLights();
			static bool Unbind();

			static bool BindDirectionalLight(unsigned int index);
			static bool BindPointLight(unsigned int index);
			static void Destroy();
			
		private:
			static ID3D11Buffer* s_lightBuffer;
		};

		

	}
}