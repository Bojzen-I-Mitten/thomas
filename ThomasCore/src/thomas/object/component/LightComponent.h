#pragma once

#include "Component.h"

#include "../../utils/Math.h"
#include "../../graphics/LightManager.h"

/**
LightComponent class
*/


namespace thomas
{
	namespace object 
	{
		namespace component
		{
			class THOMAS_API LightComponent : public Component
			{

			private:
				
			public:
				virtual bool SetLightColor(thomas::math::Vector4 other) = 0;

				virtual bool Bind() = 0;
			protected:
				LightComponent();
				~LightComponent();
				
			};


			class THOMAS_API DirectionalLight : public LightComponent
			{
			public:
				DirectionalLight();
				~DirectionalLight();

				bool SetLightColor(thomas::math::Vector4 other);

				bool Bind();

				void Update();
			private:
				int m_index;
				graphics::LightManager::DirectionalLightStruct m_thisLight;
			};

			class THOMAS_API PointLight : public LightComponent
			{
			public:
				PointLight();
				~PointLight();

				bool SetLightColor(thomas::math::Vector4 other);
				bool SetConstantAttenuation(float other);
				bool SetLinearAttenuation(float other);
				bool SetQuadraticAttenuation(float other);
				bool SetPower(float other);

				bool Bind();

				void Update();
				

			private:
				int m_index;
				graphics::LightManager::PointLightStruct m_thisLight;
			};
			
		}
	}


}