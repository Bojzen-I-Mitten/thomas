#include "GizmoTransform.h"
#include "../../object/GameObject.h"
#include "../EditorCamera.h"
#include "../../Input.h"
#include "../../ThomasTime.h"

namespace thomas
{
	namespace editor
	{
		bool GizmoTransform::m_hasCollided;
		math::Vector3 GizmoTransform::collidedAxis;

		bool GizmoTransform::CollisionTest(object::GameObject * gameObject)
		{
			float scale = math::Vector3::Distance(EditorCamera::GetEditorCamera()->m_transform->GetPosition(), gameObject->m_transform->GetPosition()) / 5.0f;
			utils::BoundingSphere xAxisBounds(scale*gameObject->m_transform->Right() + gameObject->m_transform->GetPosition(), scale*0.5f);
			utils::BoundingSphere yAxisBounds(scale*gameObject->m_transform->Up() + gameObject->m_transform->GetPosition(), scale*0.5f);
			utils::BoundingSphere zAxisBounds(scale*gameObject->m_transform->Forward() + gameObject->m_transform->GetPosition(), scale*0.5f);
			utils::Ray ray = EditorCamera::GetEditorCamera()->GetCamera()->ScreenPointToRay(Input::GetMousePosition());

			if (xAxisBounds.intersection(ray))
			{
				m_hasCollided = true;
				collidedAxis = gameObject->m_transform->Right();
			}
			else if (yAxisBounds.intersection(ray))
			{
				m_hasCollided = true;
				collidedAxis = gameObject->m_transform->Up();
			}
			else if (zAxisBounds.intersection(ray))
			{
				m_hasCollided = true;
				collidedAxis = gameObject->m_transform->Forward();
			}
			else
			{
				m_hasCollided = false;
				collidedAxis = math::Vector3();
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

