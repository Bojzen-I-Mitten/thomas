#pragma once
#include "../../Common.h"
#include "../../utils/Math.h"
#include "../../utils/Utility.h"
namespace thomas
{
	namespace object { class GameObject; }
	namespace editor
	{
		class THOMAS_API GizmoTransform
		{

		public:

			static bool m_hasCollided;
			static math::Vector3 collidedAxis;

			static bool CollisionTest(object::GameObject* gameObject);
			static void Move(object::GameObject* gameObject);
		private:
			
			//object::GameObject* m_selectedObject;
		};
	}
}
