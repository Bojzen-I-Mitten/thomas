#pragma once
#include "Component.h"
#include "../../utils/Math.h"
namespace thomas
{
	namespace object
	{
		namespace component
		{
			class Camera;
			class THOMAS_API LightComponent : public Component
			{
				
			private:
				
			public:
				enum LIGHT_TYPES
				{
					DIRECTIONAL = 0,
					POINT = 1,
					SPOT = 2
				};
				

				LightComponent();
				~LightComponent();
				void Update();

				void OnDrawGizmos();
				void SubmitToRenderer(Camera* camera);
				void OnDestroy();

				static std::vector<LightComponent*> GetAllLightComponents();
			public:
				math::BoundingSphere m_bounds;

				void SetColor(math::Color color);
				math::Color GetColor();
				void SetIntensity(float intensity);
				float GetIntensity();
				void SetSpotInnerAngle(float angle);
				float GetSpotInnerAngle();
				void SetSpotOuterAngle(float angle);
				float GetSpotOuterAngle();
				void SetConstantAttenuation(float x);
				float GetConstantAttenuation();
				void SetLinearAttenuation(float y);
				float GetLinearAttenuation();
				void SetQuadraticAttenuation(float z);
				float GetQuadraticAttenuation();
				math::Vector3 GetAttenuation();
				void SetLightType(LIGHT_TYPES type);
				LIGHT_TYPES GetType();

			private:
				LIGHT_TYPES		m_type;

				math::Color		color;
				float			intensity;
				math::Vector3	position;
				float			spotOuterAngle;
				math::Vector3	direction;
				float			spotInnerAngle;
				math::Vector3	attenuation;
			

				bool m_isStatic;


				static std::vector<LightComponent*> s_lightComponents;
			};
		}
	}
}