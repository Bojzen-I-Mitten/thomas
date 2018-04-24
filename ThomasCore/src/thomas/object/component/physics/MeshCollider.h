#pragma once

#include "Collider.h"
namespace thomas
{
	namespace resource
	{
		class Model;
	}
	namespace object
	{
		namespace component
		{
			class MeshCollider : public Collider
			{
			public:
				MeshCollider();
				resource::Model* GetMesh();
				void SetMesh(resource::Model* value);
				void OnDrawGizmosSelected();
			private:
				resource::Model* m_model = nullptr;
			};
		}
	}
}