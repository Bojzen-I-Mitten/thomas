#pragma once
#include "Common.h"
#include "../include/btBulletDynamicsCommon.h"
#include "graphics\BulletDebugDraw.h"
#include "object\component\Camera.h"
namespace thomas
{
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