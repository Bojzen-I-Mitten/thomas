#pragma once
#include "utils\Math.h"
#include <btBulletDynamicsCommon.h>
#include <memory>
#include <vector>

namespace thomas
{
	namespace graphics { class BulletDebugDraw; };
	namespace object { namespace component { class Camera; class Rigidbody; } }
	class THOMAS_API Physics
	{
	public:
		static bool Init();
		static void AddRigidBody(object::component::Rigidbody* rigidBody);
		static void RemoveRigidBody(object::component::Rigidbody* rigidBody);
		static void UpdateRigidbodies();
		static void Simulate();
		static void DrawDebug(object::component::Camera* camera);
		static void Destroy();

	public:
		static graphics::BulletDebugDraw* getDebugDraw();

	public:
		static btVector3 ToBullet(const math::Vector3& vector);
		static btQuaternion ToBullet(const math::Quaternion& quaternion);
		static math::Vector3 ToSimple(const btVector3& vector);
		static math::Quaternion ToSimple(const btQuaternion& quaternion);

	public:
		static std::unique_ptr<btDiscreteDynamicsWorld> s_world;

	private:
		static std::unique_ptr<btDefaultCollisionConfiguration> s_collisionConfiguration;
		static std::unique_ptr<btCollisionDispatcher> s_dispatcher;
		static std::unique_ptr<btBroadphaseInterface> s_broadPhase;
		static std::unique_ptr<btSequentialImpulseConstraintSolver> s_solver;
		static std::unique_ptr<graphics::BulletDebugDraw> s_debugDraw;

	private:
		static float s_timeSinceLastPhysicsStep;
		static float s_accumulator;

		static std::vector<object::component::Rigidbody*> s_rigidBodies;
	};
}