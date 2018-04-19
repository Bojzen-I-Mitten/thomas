#pragma once
#include "Component.h"
#include "../../utils/Math.h"
#include "../../utils/Buffers.h"
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
				
				struct Light
				{
					math::Vector3  color;
					float   intensity;
					math::Vector3  position;
					float   spotOuterAngle;
					math::Vector3  direction;
					float   spotInnerAngle;
					math::Vector3  attenuation;
					int type;
				};

				LightComponent();
				~LightComponent();
				void Update();

				void OnDrawGizmos();
				void SubmitToRenderer(Camera* camera);
				void OnDestroy();

				static std::vector<LightComponent*> GetAllLightComponents();
				static void UpdateLights();
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


				static void Init();
				static void Destroy();
			private:
				LIGHT_TYPES		m_type;

				math::Color		m_color;
				float			m_intensity;
				float			m_spotOuterAngle;
				float			m_spotInnerAngle;
				math::Vector3	m_attenuation;
			
				static const size_t NR_OF_LIGHTS = 32;
				static std::vector<Light> s_dataVector;
				static thomas::utils::buffers::StructuredBuffer* s_dataBuffer;
				static std::vector<LightComponent*> s_lightComponents;
			};
		}
	}
}