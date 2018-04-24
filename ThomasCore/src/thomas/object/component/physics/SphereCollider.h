#pragma once

#include "Collider.h"
namespace thomas
{
	namespace object
	{
		namespace component
		{
			class SphereCollider : public Collider
			{
			public:
				SphereCollider();

				void SetCenter(math::Vector3 center);
				math::Vector3 getCenter();
				void SetRadius(float size);
				float GetRadius();

				void OnDrawGizmosSelected();
			private:
				math::Vector3 m_center;
				float m_radius;
			};
		}
	}
}