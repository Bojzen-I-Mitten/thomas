#pragma once
#include "Common.h"
#include "utils\Math.h"

#include <btBulletDynamicsCommon.h>

namespace thomas
{
	namespace graphics { class BulletDebugDraw; };
	namespace object { namespace component { class Camera; } }
	class THOMAS_API Physics
	{
	private:
	public:
	public:
		static btDiscreteDynamicsWorld* s_world;
		static bool Init();
		static void Update();
		static void DrawDebug(object::component::Camera* camera);
		static void Destroy();
		static graphics::BulletDebugDraw* getDebugDraw();

		static btVector3 ToBullet(math::Vector3& vector);
		static math::Vector3 ToSimple(btVector3& vector);

		static btQuaternion ToBullet(math::Quaternion& quaternion);
		static math::Quaternion ToSimple(btQuaternion& quaternion);
	private:
		static graphics::BulletDebugDraw* s_debugDraw;
		static float s_accumulator;
	};
}