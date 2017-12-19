#include "GizmoTransform.h"
#include "../../object/GameObject.h"
#include "../EditorCamera.h"
#include "../../Input.h"
#include "../../ThomasTime.h"
#include "../../Physics.h"
#include "../../graphics/BulletDebugDraw.h"
namespace thomas
{
	namespace editor
	{
		bool GizmoTransform::m_hasCollided;
		math::Vector3 GizmoTransform::collidedAxis;

		bool GizmoTransform::CollisionTest(object::GameObject * gameObject)
		{
			float scale = math::Vector3::Distance(EditorCamera::GetEditorCamera()->m_transform->GetPosition(), gameObject->m_transform->GetPosition()) / 5.0f;


			math::BoundingSphere xAxisBounds(1.1f*scale*gameObject->m_transform->Right() + gameObject->m_transform->GetPosition(), scale*0.2f);
			math::BoundingSphere yAxisBounds(1.1f*scale*gameObject->m_transform->Up() + gameObject->m_transform->GetPosition(), scale*0.2f);
			math::BoundingSphere zAxisBounds(1.1f*scale*gameObject->m_transform->Forward() + gameObject->m_transform->GetPosition(), scale*0.2f);
			math::Ray ray = EditorCamera::GetEditorCamera()->GetCamera()->ScreenPointToRay(Input::GetMousePosition());
			/*Physics::getDebugDraw()->drawLine
			(
				btVector3(ray.origin.x+0.01f, ray.origin.y + 0.01f, ray.origin.z + 0.01f),
				btVector3(ray.origin.x + ray.direction.x*1000, ray.origin.y + ray.direction.y*1000, ray.origin.z + ray.direction.z * 1000), btVector3(1, 0, 0)
			);*/
			float dist;
			if (ray.Intersects(xAxisBounds, dist))
			{
				m_hasCollided = true;
				collidedAxis = gameObject->m_transform->Right();
			}
			else if (ray.Intersects(yAxisBounds, dist))
			{
				m_hasCollided = true;
				collidedAxis = gameObject->m_transform->Up();
			}
			else if (ray.Intersects(zAxisBounds, dist))
			{
				m_hasCollided = true;
				collidedAxis = gameObject->m_transform->Forward();
			}
			else
			{
				m_hasCollided = false;
				collidedAxis = math::Vector3::Zero;
			}

			return m_hasCollided;

		}

		void GizmoTransform::Move(object::GameObject * gameObject)
		{
			
			Input::SetMouseMode(Input::MouseMode::POSITION_RELATIVE);
			float delta = Input::GetMouseX() - Input::GetMouseY();

			gameObject->m_transform->Translate(collidedAxis*delta* ThomasTime::GetActualDeltaTime());
			gameObject->m_transform->SetDirty(true);
		}

	}
}

