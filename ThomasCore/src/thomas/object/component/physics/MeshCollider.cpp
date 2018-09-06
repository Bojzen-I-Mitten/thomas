#include "MeshCollider.h"
#include "../../../resource/Model.h"
#include <BulletCollision\CollisionShapes\btShapeHull.h>
#include "../../../graphics/Mesh.h"
namespace thomas
{

	namespace object
	{
		namespace component
		{
			MeshCollider::MeshCollider() : Collider(new btConvexHullShape())
			{
			}
			resource::Model * MeshCollider::GetMesh()
			{
				return m_model;
			}
			void MeshCollider::SetMesh(resource::Model * value)
			{
				m_model = value;
				btConvexHullShape* temp = new btConvexHullShape();
				for (auto mesh : value->GetMeshes())
				{

					for (int i = 0; i < mesh->GetVertexCount(); i++)
					{
						math::Vector4 v = mesh->GetVertices().positions[i];
						temp->addPoint(btVector3(v.x, v.y, v.z));
					}
				}
				btShapeHull* hull = new btShapeHull(temp);
				hull->buildHull(1.0f);
			
				
				const btVector3* hullPtr = hull->getVertexPointer();
				for (int i = 0; i < hull->numVertices(); i++)
					((btConvexHullShape*)m_collisionShape)->addPoint(hullPtr[i], false);


				((btConvexHullShape*)m_collisionShape)->recalcLocalAabb();

				delete temp;
				delete hull;
			}
			void MeshCollider::OnDrawGizmosSelected()
			{
				//((btConvexHullShape*)m_collisionShape)->get
			}
		}
	}
}

