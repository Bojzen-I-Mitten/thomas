#include "Physics.h"
#include "ThomasTime.h"
#include "object\component\RigidBodyComponent.h"
#include "object\GameObject.h"
#include "graphics\BulletDebugDraw.h"
#include "object\component\Camera.h"
#define PHYSICS_TIMESTEP 1.0f/60.0f
namespace thomas
{
	graphics::BulletDebugDraw* Physics::s_debugDraw;
	btDiscreteDynamicsWorld* Physics::s_world;
	float Physics::s_accumulator;
	bool Physics::Init()
	{
		// collision configuration contains default setup for memory, collision setup. Advanced
	    //  users can create their own configuration.
		btDefaultCollisionConfiguration * collisionConfiguration = new btDefaultCollisionConfiguration();
		// use the default collision dispatcher. For parallel processing you can use a diffent
		//dispatcher(see Extras / BulletMultiThreaded)
		btCollisionDispatcher * dispatcher = new btCollisionDispatcher(collisionConfiguration);
		
		// btDbvtBroadphase is a good general purpose broadphase. You can also try out
		//btAxis3Sweep.
		btBroadphaseInterface * overlappingPairCache = new btDbvtBroadphase();
		
		// the default constraint solver. For parallel processing you can use a different solver
		//(see Extras / BulletMultiThreaded)
		btSequentialImpulseConstraintSolver * solver = new btSequentialImpulseConstraintSolver;

		s_world = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
		s_world->setGravity(btVector3(0, -9.82f, 0));

		s_debugDraw = new graphics::BulletDebugDraw();

		s_debugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
		s_world->setDebugDrawer(s_debugDraw);
		

		return s_debugDraw->m_initialized;

	}
	void Physics::Update()
	{
		s_world->stepSimulation(ThomasTime::GetDeltaTime(), 7); 

		int numManifolds = s_world->getDispatcher()->getNumManifolds();
		for (int i = 0; i < numManifolds; i++)
		{
			if (numManifolds > s_world->getDispatcher()->getNumManifolds())
			{
				break;
			}
				
			btPersistentManifold* contactManifold = s_world->getDispatcher()->getManifoldByIndexInternal(i);
			btCollisionObject* obA = (btCollisionObject*)contactManifold->getBody0();
			btCollisionObject* obB = (btCollisionObject*)contactManifold->getBody1();
			
			object::component::RigidBodyComponent* rbA = static_cast<object::component::RigidBodyComponent*>(obA);
			object::component::RigidBodyComponent* rbB = static_cast<object::component::RigidBodyComponent*>(obB);

			if (rbA->isActive() && rbB->isActive())
			{
				object::component::RigidBodyComponent::Collision colA;
				object::component::RigidBodyComponent::Collision colB;
				colA.thisRigidbody = rbA;
				colA.otherRigidbody = rbB;
				colB.thisRigidbody = rbB;
				colB.otherRigidbody = rbA;
				/*rbA->m_gameObject->OnCollision(colA);
				rbB->m_gameObject->OnCollision(colB);*/
			}
				
		}
		

	}
	void Physics::DrawDebug(object::component::Camera* camera)
	{
		s_debugDraw->Update(camera);
		s_world->debugDrawWorld();
		s_debugDraw->drawLineFinal();
	}
	void Physics::Destroy()
	{
		//Destroy everything????
		btCollisionObjectArray arr = s_world->getCollisionObjectArray();
		arr.clear();
		SAFE_DELETE(s_debugDraw);
		
	}

	graphics::BulletDebugDraw * Physics::getDebugDraw()
	{
		if (s_debugDraw)
			return s_debugDraw;
		else
			return nullptr;
	}

	btVector3 Physics::ToBullet(math::Vector3 & vector)
	{
		return *(btVector3*)&vector;
	}

	math::Vector3 Physics::ToSimple(btVector3 & vector)
	{
		return (math::Vector3)vector;
	}
	btQuaternion Physics::ToBullet(math::Quaternion& quaternion)
	{
		return *(btQuaternion*)&quaternion;
	}

	math::Quaternion Physics::ToSimple(btQuaternion & quaternion)
	{
		return (math::Quaternion)quaternion;
	}



}
