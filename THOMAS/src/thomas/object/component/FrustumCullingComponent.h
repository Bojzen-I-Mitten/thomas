#pragma once
#include "Component.h"
#include "../../utils/Math.h"

namespace thomas
{
	namespace object
	{
		namespace component
		{
			class THOMAS_API FrustumCullingComponent : public Component
			{
			public:
				FrustumCullingComponent();
				void Update();

				void SetRadius(float radius);
				void SetPosition(math::Vector3 position);
				float GetRadius();
				math::Vector3 GetPosition();

			private:
				float m_radius;
				math::Vector3 m_position;
			};


		}
	}
}