#include "Physics.h"
#include "ThomasTime.h"
#include "object\component\physics\Rigidbody.h"
#include "object\GameObject.h"
#include "object\component\Camera.h"
#include "graphics\BulletDebugDraw.h"

namespace thomas
{
	std::unique_ptr<btDefaultCollisionConfiguration> Physics::s_collisionConfiguration;
	std::unique_ptr<btCollisionDispatcher> Physics::s_dispatcher;
	std::unique_ptr<btBroadphaseInterface> Physics::s_broadPhase;
	std::unique_ptr<btSequentialImpulseConstraintSolver> Physics::s_solver;
	std::unique_ptr<graphics::BulletDebugDraw> Physics::s_debugDraw;
	std::unique_ptr<btDiscreteDynamicsWorld> Physics::s_world;
	float Physics::s_timeStep = 1.0f / 60.0f; //Limit physics timestep to 60 FPS
	float Physics::s_timeSinceLastPhysicsStep = 0.0f;
	std::vector<object::component::Rigidbody*> Physics::s_rigidBodies;
	float Physics::s_accumulator;
	std::vector<object::component::RigidBodyComponent*> Physics::s_rigidBodies;

	bool Physics::Init()
	{
		//Init Bullet
		s_collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
		s_dispatcher = std::make_unique<btCollisionDispatcher>(s_collisionConfiguration.get());
		s_broadPhase = std::make_unique<btDbvtBroadphase>();
		s_solver = std::make_unique<btSequentialImpulseConstraintSolver>();
		s_world = std::make_unique<btDiscreteDynamicsWorld>(s_dispatcher.get(), s_broadPhase.get(), s_solver.get(), s_collisionConfiguration.get());
		s_debugDraw = std::make_unique<graphics::BulletDebugDraw>();

		//Set states
		s_world->setGravity(btVector3(0, -9.82f, 0));
		s_debugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
		s_world->setDebugDrawer(s_debugDraw.get());

		return true;
	}
	void Physics::AddRigidBody(object::component::Rigidbody * rigidBody)
	{
		s_rigidBodies.push_back(rigidBody);
		s_world->addRigidBody(rigidBody);
	}
	void Physics::RemoveRigidBody(object::component::Rigidbody * rigidBody)
	{
		for (unsigned i = 0; i < s_rigidBodies.size(); ++i)
		{
			object::component::Rigidbody* rb = s_rigidBodies[i];
			if (rb == rigidBody)
			{
				s_rigidBodies.erase(s_rigidBodies.begin() + i);
				break;
			}
		}
		s_world->removeRigidBody(rigidBody);		
	}

	void Physics::UpdateRigidbodies()
	{
		for (object::component::Rigidbody* rb : s_rigidBodies)
		{
			rb->UpdateTransformToRigidBody();
	}

	//Update physics collision
	void Physics::Simulate()
	{
		s_timeSinceLastPhysicsStep += ThomasTime::GetDeltaTime();

		if (s_timeSinceLastPhysicsStep < s_timeStep)
			return;

		s_world->stepSimulation(s_timeSinceLastPhysicsStep, 5, s_timeStep);
		int numManifolds = s_world->getDispatcher()->getNumManifolds();

		for (unsigned i = 0; i < numManifolds; ++i)
		{
			if (numManifolds > s_world->getDispatcher()->getNumManifolds())
				break;
				
			btPersistentManifold* contactManifold = s_world->getDispatcher()->getManifoldByIndexInternal(i);
			btCollisionObject* obA = (btCollisionObject*)contactManifold->getBody0();
			btCollisionObject* obB = (btCollisionObject*)contactManifold->getBody1();
			object::component::Rigidbody* rbA = static_cast<object::component::Rigidbody*>(obA);
			object::component::Rigidbody* rbB = static_cast<object::component::Rigidbody*>(obB);

			if (rbA->isActive() && rbB->isActive())
			{
				object::component::Rigidbody::Collision colA;
				object::component::Rigidbody::Collision colB;
				colA.thisRigidbody = rbA;
				colA.otherRigidbody = rbB;
				colB.thisRigidbody = rbB;
				colB.otherRigidbody = rbA;
				/*rbA->m_gameObject->OnCollision(colA);
				rbB->m_gameObject->OnCollision(colB);*/
			}
				
		}
		for (object::component::Rigidbody* rb : s_rigidBodies)
		{
			rb->UpdateRigidbodyToTransform();

		s_timeSinceLastPhysicsStep = 0.f;
	}

	void Physics::DrawDebug(object::component::Camera* camera)
	{
		s_debugDraw->Update(camera);
		s_world->debugDrawWorld();
		s_debugDraw->drawLineFinal();
	}

	void Physics::Destroy()
	{
		btCollisionObjectArray arr = s_world->getCollisionObjectArray();
		arr.clear();
		s_collisionConfiguration.reset();
		s_debugDraw.reset();
		s_dispatcher.reset();
		s_solver.reset();
		s_world.reset();		
	}

	graphics::BulletDebugDraw * Physics::getDebugDraw()
	{
		if (s_debugDraw)
			return s_debugDraw.get();
		else
			return nullptr;
	}

	btVector3 Physics::ToBullet(const math::Vector3 & vector)
	{
		return *(btVector3*)&vector;
	}

	math::Vector3 Physics::ToSimple(const btVector3 & vector)
	{
		return (math::Vector3)vector;
	}

	btQuaternion Physics::ToBullet(const math::Quaternion& quaternion)
	{
		return *(btQuaternion*)&quaternion;
	}

	math::Quaternion Physics::ToSimple(const btQuaternion & quaternion)
	{
		return (math::Quaternion)quaternion;
	}
}
