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
				std::vector<math::Vector4> points;
				for (auto mesh : value->GetMeshes())
				{
					/*points.reserve(points.size() + mesh->GetVertexCount());
					points.insert(points.end(), mesh->GetVertices()->positions.begin(), mesh->GetVertices()->positions.end());
*/

					for (int i = 0; i < mesh->GetIndexCount(); i++)
					{
						int index = mesh->GetIndices()->at(i);
						math::Vector4 v = mesh->GetVertices()->positions[index];
						((btConvexHullShape*)m_collisionShape)->addPoint(btVector3(v.x, v.y, v.z));
					}
				}
				//btConvexHullShape* og = new btConvexHullShape((float*)points.data(), points.size() * 4);
				//btShapeHull* hull = new btShapeHull(og);
				//btScalar margin = og->getMargin();
				//hull->buildHull(margin);
			
				//const btVector3* hullPtr = hull->getVertexPointer();
				//for (int i = 0; i < hull->numVertices(); i++)
				//	((btConvexHullShape*)m_collisionShape)->addPoint(hullPtr[i]);

				

				((btConvexHullShape*)m_collisionShape)->recalcLocalAabb();

				//delete og;
				//delete hull;
			}
			void MeshCollider::OnDrawGizmosSelected()
			{
				//((btConvexHullShape*)m_collisionShape)->get
			}
		}
	}
}

