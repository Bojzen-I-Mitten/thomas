#include "EditorCamera.h"
#include "../utils/DebugTools.h"
#include "../Input.h"
#include "../ThomasTime.h"
#include <algorithm>
namespace thomas
{
	namespace editor
	{
		EditorCamera* EditorCamera::s_editorCamera = nullptr;


		EditorCamera::EditorCamera() : GameObject("editorCamera")
		{
			m_transform = new object::component::Transform();
			m_transform->m_gameObject = this;
			m_cameraComponent = new object::component::Camera();
			m_cameraComponent->m_gameObject = this;
		}

		void EditorCamera::Init()
		{
			s_editorCamera = new EditorCamera();
		}

		EditorCamera * EditorCamera::GetEditorCamera()
		{
			return s_editorCamera;
		}

		void EditorCamera::Render()
		{
			if (s_editorCamera)
				s_editorCamera->renderCamera();
		}
		void EditorCamera::Update()
		{
			if (s_editorCamera)
				s_editorCamera->updateCamera();
		}
		void EditorCamera::renderCamera()
		{
			m_cameraComponent->Render();
		}
		void EditorCamera::updateCamera()
		{
			if (Input::GetMouseButtonDown(Input::MouseButtons::RIGHT))
			{
				Input::SetMouseMode(Input::MouseMode::POSITION_RELATIVE);
			}
			
			if (Input::GetMouseButtonUP(Input::MouseButtons::RIGHT))
				Input::SetMouseMode(Input::MouseMode::POSITION_ABSOLUTE);

			if (Input::GetKeyDown(Input::Keys::C))
			{
				m_transform->SetRotation(math::Quaternion::Identity);
				rotationX = 0;
				rotationY = 0;
			}

			if (Input::GetKey(Input::Keys::A))
				m_transform->Translate(-m_transform->Right()*ThomasTime::GetActualDeltaTime());
			if (Input::GetKey(Input::Keys::D))
				m_transform->Translate(m_transform->Right()*ThomasTime::GetActualDeltaTime());
				
			if (Input::GetKey(Input::Keys::W))
				m_transform->Translate(m_transform->Forward()*ThomasTime::GetActualDeltaTime());

			if (Input::GetKey(Input::Keys::S))
				m_transform->Translate(-m_transform->Forward()*ThomasTime::GetActualDeltaTime());

			if (Input::GetKey(Input::Keys::Q))
				m_transform->Translate(m_transform->Up()*ThomasTime::GetActualDeltaTime());

			if (Input::GetKey(Input::Keys::E))
				m_transform->Translate(-m_transform->Up()*ThomasTime::GetActualDeltaTime());

			

			if (Input::GetMouseButton(Input::MouseButtons::RIGHT))
			{
				rotationX += Input::GetMouseX() * ThomasTime::GetActualDeltaTime();
				rotationY += Input::GetMouseY() * ThomasTime::GetActualDeltaTime();
				rotationY = max(-90.0f, min(rotationY, 90.0f));
				

				math::Quaternion rotation = math::Quaternion::CreateFromAxisAngle(-math::Vector3::Up, math::DegreesToRadians(rotationX));
				rotation *= math::Quaternion::CreateFromAxisAngle(math::Vector3::Left, math::DegreesToRadians(rotationY));
				m_transform->SetRotation(rotation);
			}
		}
	}
}

