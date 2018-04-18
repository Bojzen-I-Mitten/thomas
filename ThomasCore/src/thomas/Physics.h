#pragma once
#include "Common.h"
#include "utils\Math.h"

#include <btBulletDynamicsCommon.h>

namespace thomas
{
	namespace graphics { class BulletDebugDraw; };
	namespace object { namespace component { class Camera; class Rigidbody; } }
	class THOMAS_API Physics
	{
	private:
	public:
	public:
		static btDiscreteDynamicsWorld* s_world;
		static float s_timeStep;
		static bool Init();
		static void AddRigidBody(object::component::Rigidbody* rigidBody);
		static void RemoveRigidBody(object::component::Rigidbody* rigidBody);
		static void UpdateRigidbodies();
		static void Simulate();
		static void DrawDebug(object::component::Camera* camera);
		static void Destroy();
		static graphics::BulletDebugDraw* getDebugDraw();

		static btVector3 ToBullet(math::Vector3& vector);
		static math::Vector3 ToSimple(btVector3& vector);

		static btQuaternion ToBullet(math::Quaternion& quaternion);
		static math::Quaternion ToSimple(btQuaternion& quaternion);
	private:
		static float s_timeSinceLastPhysicsStep;
		static graphics::BulletDebugDraw* s_debugDraw;
		static float s_accumulator;

		static std::vector<object::component::Rigidbody*> s_rigidBodies;
	};
}