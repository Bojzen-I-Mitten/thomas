#include "EditorCamera.h"
#include "../utils/DebugTools.h"
#include "../Input.h"
#include "../ThomasTime.h"
#include <algorithm>
#include "../Window.h"
#include "../graphics/BulletDebugDraw.h"
#include "EditorGrid.h"
#include "../graphics/Shader.h"
#include "../graphics/Material.h"
#include "../graphics/Renderer.h"
#include "../graphics/Model.h"
namespace thomas
{
	namespace editor
	{
		EditorCamera* EditorCamera::s_editorCamera = nullptr;
		std::vector<object::GameObject*> EditorCamera::s_selectedObjects;
		
		EditorCamera::EditorCamera() : GameObject("editorCamera")
		{
			m_transform = new object::component::Transform();
			m_transform->m_gameObject = this;
			m_cameraComponent = new object::component::Camera(true);
			m_cameraComponent->SetTargetDisplay(-1);
			m_cameraComponent->m_gameObject = this;
			m_grid = new EditorGrid(30, 1,  10);
			m_sensitivity = 30.0f;

			m_objectHighlighter = nullptr;
			graphics::Shader* outliner = graphics::Shader::CreateShader("ediotrOutliner", "../Data/FXIncludes/EditorOutlineShader.fx");
			if (outliner)
				m_objectHighlighter = new graphics::Material(outliner);
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
		void EditorCamera::SelectObject(GameObject * gameObject)
		{
			s_selectedObjects.clear();
			s_selectedObjects.push_back(gameObject);
		}
		void EditorCamera::renderCamera()
		{		
			thomas::graphics::Renderer::BindCamera(m_cameraComponent);
			m_grid->Draw(m_cameraComponent->GetPosition());
			renderSelectedObjects();
			thomas::graphics::Renderer::Render();			
		}
		void EditorCamera::updateCamera()
		{
			if (!Window::GetEditorWindow() || Window::GetEditorWindow()->GetWindowHandler() != GetFocus())
				return;

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
				m_transform->Translate(-m_transform->Up()*ThomasTime::GetActualDeltaTime());

			if (Input::GetKey(Input::Keys::E))
				m_transform->Translate(m_transform->Up()*ThomasTime::GetActualDeltaTime());

			

			if	(Input::GetMouseButton(Input::MouseButtons::RIGHT))
			{
				rotationX += Input::GetMouseX() * ThomasTime::GetActualDeltaTime() * m_sensitivity;
				rotationY += Input::GetMouseY() * ThomasTime::GetActualDeltaTime() * m_sensitivity;
				
				m_transform->SetRotation(-rotationX, -rotationY, 0);
			}
		}
		void EditorCamera::renderSelectedObjects()
		{
			if (!m_objectHighlighter)
				return;
			for(GameObject* gameObject : s_selectedObjects)
			{
				graphics::Renderer::BindObject(m_objectHighlighter, gameObject->m_transform);
				m_objectHighlighter->Bind();
				object::component::RenderComponent* renderComponent = gameObject->GetComponent<object::component::RenderComponent>();
				if (renderComponent)
				{
					graphics::Model* model = gameObject->GetComponent<object::component::RenderComponent>()->GetModel();
					if (model)
					{
						for (graphics::Mesh* mesh : model->GetMeshes())
							m_objectHighlighter->Draw(mesh);
					}
				}				
			}
				
		}
	}
}

