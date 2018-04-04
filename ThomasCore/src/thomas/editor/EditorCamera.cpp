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
			m_sensitivity = 50.0f;
			m_speed = 2.0f;
			m_hasSelectionChanged = false;
			m_objectHighlighter = nullptr;
			graphics::Shader* outliner = graphics::Shader::CreateShader("editorOutliner", "../Data/FXIncludes/EditorOutlineShader.fx");
			if (outliner)
				m_objectHighlighter = new graphics::Material(outliner);

			m_transform->SetPosition(math::Vector3(5, 5, 5));
			m_transform->LookAt(math::Vector3(0, 0, 0));
			math::Vector3 eulerAngles = math::ToEuler(m_transform->GetRotation());
			rotationX = -eulerAngles.y;
			rotationY = -eulerAngles.x;
			m_transform->SetRotation(-rotationX, -rotationY, 0);

			m_manipulatorMode = ImGuizmo::MODE::LOCAL;
			m_manipulatorOperation = ImGuizmo::OPERATION::TRANSLATE;
		}

		EditorCamera::~EditorCamera()
		{
			SAFE_DELETE(m_transform);
			delete m_cameraComponent;
			delete m_grid;
		}

		void EditorCamera::Destroy()
		{
			if (s_editorCamera)
				delete s_editorCamera;
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
		void EditorCamera::UnselectObject(GameObject * gameObject)
		{
			for (int i = 0; i < s_selectedObjects.size(); i++)
			{
				if (s_selectedObjects[i] == gameObject)
				{
					s_selectedObjects.erase(s_selectedObjects.begin() + i);
					--i;
				}
			}
		}
		void EditorCamera::UnselectObjects()
		{
			s_selectedObjects.clear();
			GetEditorCamera()->m_hasSelectionChanged = true;
		}
		std::vector<object::GameObject*> EditorCamera::GetSelectedObjects()
		{
			return s_selectedObjects;
		}

		void EditorCamera::SetHasSelectionChanged(bool selectionChanged)
		{
			GetEditorCamera()->m_hasSelectionChanged = selectionChanged;
		}

		bool EditorCamera::HasSelectionChanged()
		{
			return GetEditorCamera()->m_hasSelectionChanged;
		}
		object::component::Camera * EditorCamera::GetCamera()
		{
			return m_cameraComponent;
		}
		void EditorCamera::SetManipulatorOperation(ImGuizmo::OPERATION operation)
		{
			s_editorCamera->m_manipulatorOperation = operation;
		}
		ImGuizmo::OPERATION EditorCamera::GetManipulatorOperation()
		{
			return s_editorCamera->m_manipulatorOperation;
		}
		void EditorCamera::renderCamera()
		{		
			thomas::graphics::Renderer::BindCamera(m_cameraComponent);
			
			renderSelectedObjects();
			thomas::graphics::Renderer::Render();
			renderGizmos();
			Physics::DrawDebug(m_cameraComponent);
			m_grid->Draw(m_cameraComponent->GetPosition());

			
		}
		void EditorCamera::updateCamera()
		{
			m_manipulatorSnapping = false;
			HWND focus = GetForegroundWindow();

			if (!Window::GetEditorWindow())
				return;

				
			
			if (Input::GetMouseButtonDown(Input::MouseButtons::RIGHT))
			{
				Input::SetMouseMode(Input::MouseMode::POSITION_RELATIVE);
			}
			
			if (Input::GetMouseButtonUp(Input::MouseButtons::RIGHT))
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
				if (!ImGuizmo::IsOver())
				{
					object::GameObject* gObj = findClickedGameObject();
					SelectObject(gObj);
				}
				
			}
			else
			{
				Input::SetMouseMode(Input::MouseMode::POSITION_ABSOLUTE);
				
				if (Input::GetKeyDown(Input::Keys::W))
					m_manipulatorOperation = ImGuizmo::OPERATION::TRANSLATE;

				if (Input::GetKeyDown(Input::Keys::R))
					m_manipulatorOperation = ImGuizmo::OPERATION::ROTATE;

				if (Input::GetKeyDown(Input::Keys::E))
					m_manipulatorOperation = ImGuizmo::OPERATION::SCALE;

				if (Input::GetKey(Input::Keys::LeftShift))
					m_manipulatorSnapping = true;
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
		void EditorCamera::renderGizmos()
		{
			for (int i=0; i < s_selectedObjects.size(); i++)
			{
				object::GameObject* gameObject = s_selectedObjects[i];

				ImGuiIO& io = ImGui::GetIO();
				
				ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

				math::Matrix worldMatrix = gameObject->m_transform->GetLocalWorldMatrix();

				float bounds[] = { 5,5,5 };
				float snap[] = { 1,1,1 };

				if (m_manipulatorOperation == ImGuizmo::OPERATION::ROTATE)
					snap[0] = 15;

				ImGuizmo::Manipulate(
					*(m_cameraComponent->GetViewMatrix() * math::Matrix::CreateScale(m_manipulatorScale)).m, *m_cameraComponent->GetProjMatrix().m,
					m_manipulatorOperation, m_manipulatorMode, *worldMatrix.m, 0, m_manipulatorSnapping ? snap : 0);

				if (worldMatrix != gameObject->m_transform->GetLocalWorldMatrix())
				{
					gameObject->m_transform->SetLocalMatrix(worldMatrix);
					gameObject->m_transform->SetDirty(true);
				}
			
			}

		}
		object::GameObject* EditorCamera::findClickedGameObject()
		{
			math::Ray ray = m_cameraComponent->ScreenPointToRay(Input::GetMousePosition());

			std::vector<object::component::RenderComponent*> renderComponents = object::Object::FindObjectsOfType<object::component::RenderComponent>();

			object::GameObject* closestGameObject = nullptr;
			float closestDistance = m_cameraComponent->GetFar();
			for (object::component::RenderComponent* renderComponent : renderComponents)
			{
				float distance;
				if (renderComponent->m_bounds.Intersects(ray.position, ray.direction, distance) && distance < closestDistance)
				{
					closestDistance = distance;
					closestGameObject = renderComponent->m_gameObject;
				}
			}
			return closestGameObject;
			
		}
	}
}

