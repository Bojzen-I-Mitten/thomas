#pragma once

#include "Collider.h"
namespace thomas
{
	namespace object
	{
		namespace component
		{
			class BoxCollider : public Collider
			{
			public:
				BoxCollider();

				void SetCenter(math::Vector3 center);
				math::Vector3 getCenter();
				void SetSize(math::Vector3 size);
				math::Vector3 getSize();

				void OnDrawGizmosSelected();
			private:
				math::Vector3 m_center;
				math::Vector3 m_size;
			};
		}
	}
}