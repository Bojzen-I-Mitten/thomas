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
		
		EditorCamera::EditorCamera() : object::GameObject("editorCamera")
		{
			m_transform = new object::component::Transform();
			m_transform->m_gameObject = this;
			m_cameraComponent = new object::component::Camera(true);
			m_cameraComponent->SetTargetDisplay(-1);
			m_cameraComponent->m_gameObject = this;
			m_grid = new EditorGrid(100, 1,  10);
			m_sensitivity = 30.0f;
			m_speed = 2.0f;
			m_hasSelectionChanged = false;
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
		void EditorCamera::SelectObject(object::GameObject * gameObject)
		{
			s_selectedObjects.clear();
			if(gameObject)
				s_selectedObjects.push_back(gameObject);
			GetEditorCamera()->m_hasSelectionChanged = true;
		}
		std::vector<object::GameObject*> EditorCamera::GetSelectedObjects()
		{
			return s_selectedObjects;
		}
		bool EditorCamera::HasSelectionChanged()
		{
			return GetEditorCamera()->m_hasSelectionChanged;
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
			m_hasSelectionChanged = false;
			if (!Window::GetEditorWindow() || Window::GetEditorWindow()->GetWindowHandler() != GetFocus())
				return;

			if (Input::GetMouseButtonDown(Input::MouseButtons::RIGHT))
			{
				Input::SetMouseMode(Input::MouseMode::POSITION_RELATIVE);
			}
			
			if (Input::GetMouseButtonUP(Input::MouseButtons::RIGHT))
				Input::SetMouseMode(Input::MouseMode::POSITION_ABSOLUTE);
			

			if	(Input::GetMouseButton(Input::MouseButtons::RIGHT))
			{
				float speed = m_speed;

				if (Input::GetKey(Input::Keys::LeftShift))
					speed *= 4.0f;

				if (Input::GetKey(Input::Keys::A))
					m_transform->Translate(-m_transform->Right()*ThomasTime::GetActualDeltaTime()*speed);
				if (Input::GetKey(Input::Keys::D))
					m_transform->Translate(m_transform->Right()*ThomasTime::GetActualDeltaTime()*speed);

				if (Input::GetKey(Input::Keys::W))
					m_transform->Translate(m_transform->Forward()*ThomasTime::GetActualDeltaTime()*speed);

				if (Input::GetKey(Input::Keys::S))
					m_transform->Translate(-m_transform->Forward()*ThomasTime::GetActualDeltaTime()*speed);

				if (Input::GetKey(Input::Keys::Q))
					m_transform->Translate(-m_transform->Up()*ThomasTime::GetActualDeltaTime()*speed);

				if (Input::GetKey(Input::Keys::E))
					m_transform->Translate(m_transform->Up()*ThomasTime::GetActualDeltaTime()*speed);



				rotationX += Input::GetMouseX() * ThomasTime::GetActualDeltaTime() * m_sensitivity;
				rotationY += Input::GetMouseY() * ThomasTime::GetActualDeltaTime() * m_sensitivity;
				
				m_transform->SetRotation(-rotationX, -rotationY, 0);
			}
			else if (Input::GetMouseButtonDown(Input::MouseButtons::LEFT))
			{
				object::GameObject* gObj = findClickedGameObject();
				SelectObject(gObj);
			}
		}
		void EditorCamera::renderSelectedObjects()
		{
			if (!m_objectHighlighter)
				return;
			for(object::GameObject* gameObject : s_selectedObjects)
			{
				graphics::Renderer::BindObject(m_objectHighlighter, gameObject->m_transform);
				math::Matrix test = math::Matrix::CreateScale(1.03f) * gameObject->m_transform->GetWorldMatrix();
				m_objectHighlighter->SetMatrix("thomas_ObjectToWorld", test.Transpose());
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
		object::GameObject* EditorCamera::findClickedGameObject()
		{
			utils::Ray ray = m_cameraComponent->ScreenPointToRay(Input::GetMousePosition());

			std::vector<object::component::RenderComponent*> renderComponents = object::Object::FindObjectsOfType<object::component::RenderComponent>();

			for (float i = m_cameraComponent->GetNear(); i < m_cameraComponent->GetFar(); i += 0.1f)
			{
				for (object::component::RenderComponent* renderComponent : renderComponents)
				{
					if (renderComponent->m_bounds->Contains(ray.GetPoint(i)))
					{
						return renderComponent->m_gameObject;
					}
					
				}
			}
			return nullptr;
			
		}
	}
}

