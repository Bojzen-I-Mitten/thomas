#pragma once

#pragma unmanaged
#include <thomas\object\component\LightComponent.h>
#pragma managed
#include "../Component.h"
#include "../../math/Math.h"

namespace ThomasEditor
{
	[ExecuteInEditor]
	public ref class LightComponent : public Component
	{
	private:

	public:
		enum class LIGHT_TYPES
		{
			DIRECTIONAL = 0,
			POINT = 1,
			SPOT = 2
		};

		LightComponent() : Component(new thomas::object::component::LightComponent()) {
		}

		property Color color {
			Color get() {
				return Color(((thomas::object::component::LightComponent*)nativePtr)->GetColor());
			}
			void set(Color value) {
				((thomas::object::component::LightComponent*)nativePtr)->SetColor(thomas::math::Color(value.r, value.g, value.b));
			}
		}
		property float Intensity {
			float get() {
				return ((thomas::object::component::LightComponent*)nativePtr)->GetIntensity();
			}
			void set(float value) {
				((thomas::object::component::LightComponent*)nativePtr)->SetIntensity(value);
			}
		}
		property float SpotInnerAngle {
			float get() {
				return ((thomas::object::component::LightComponent*)nativePtr)->GetSpotInnerAngle();
			}
			void set(float value) {
				((thomas::object::component::LightComponent*)nativePtr)->SetSpotInnerAngle(value);
			}
		}
		property float SpotOuterAngle {
			float get() {
				return ((thomas::object::component::LightComponent*)nativePtr)->GetSpotOuterAngle();
			}
			void set(float value) {
				((thomas::object::component::LightComponent*)nativePtr)->SetSpotOuterAngle(value);
			}
		}
		property float ConstantAttenuation {
			float get() {
				return ((thomas::object::component::LightComponent*)nativePtr)->GetConstantAttenuation();
			}
			void set(float value) {
				((thomas::object::component::LightComponent*)nativePtr)->SetConstantAttenuation(value);
			}
		}
		property float LinearAttenuation {
			float get() {
				return ((thomas::object::component::LightComponent*)nativePtr)->GetLinearAttenuation();
			}
			void set(float value) {
				((thomas::object::component::LightComponent*)nativePtr)->SetLinearAttenuation(value);
			}
		}
		property float QuadraticAttenuation {
			float get() {
				return ((thomas::object::component::LightComponent*)nativePtr)->GetQuadraticAttenuation();
			}
			void set(float value) {
				((thomas::object::component::LightComponent*)nativePtr)->SetQuadraticAttenuation(value);
			}
		}
		property LIGHT_TYPES Type {
			LIGHT_TYPES get() {
				return LIGHT_TYPES(((thomas::object::component::LightComponent*)nativePtr)->GetType());
			}
			void set(LIGHT_TYPES value)	{
				((thomas::object::component::LightComponent*)nativePtr)->SetLightType((thomas::object::component::LightComponent::LIGHT_TYPES)value);
			}
		}


		void Update() override
		{
			((thomas::object::component::LightComponent*)nativePtr)->Update();
		}
	};
}