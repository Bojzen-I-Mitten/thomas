#include "EditorCamera.h"
#include "EditorGrid.h"
#include "..\Input.h"
#include "..\ThomasTime.h"
#include "..\Window.h"
#include "..\resource\Shader.h"
#include "..\resource\Material.h"
#include "..\graphics\Renderer.h"
#include "..\resource\Model.h"

namespace thomas
{
	namespace editor
	{
		EditorCamera* EditorCamera::s_editorCamera;
		std::vector<object::GameObject*> EditorCamera::s_selectedObjects;

		EditorCamera::EditorCamera() : object::GameObject("editorCamera"), m_sensitivity(50.f), m_speed(2.f), m_hasSelectionChanged(false), 
																		   m_objectHighlighter(nullptr), m_manipulatorMode(ImGuizmo::MODE::LOCAL),
																		   m_manipulatorOperation(ImGuizmo::OPERATION::TRANSLATE)
		{
			//Transform component
			m_transform = new object::component::Transform();
			m_transform->m_gameObject = this;
			m_transform->SetPosition(math::Vector3(5.f));
			m_transform->LookAt(math::Vector3());
			math::Vector3 eulerAngles = math::ToEuler(m_transform->GetRotation());
			m_rotationX = -eulerAngles.y;
			m_rotationY = -eulerAngles.x;
			m_transform->SetRotation(-m_rotationX, -m_rotationY, 0.f);

			//Camera component
			m_cameraComponent = std::make_unique<object::component::Camera>(true);
			m_cameraComponent->SetTargetDisplay(-1);
			m_cameraComponent->m_gameObject = this;

			//Create the grid
			m_grid = std::make_unique<EditorGrid>(100, 1.f, 10);
			resource::Shader* outliner = resource::Shader::CreateShader("../Data/FXIncludes/EditorOutlineShader.fx"); //Hardcoded outline shader
			if (outliner != nullptr)
				m_objectHighlighter = std::make_unique<resource::Material>(outliner);
		}

		EditorCamera::~EditorCamera()
		{
			SAFE_DELETE(m_transform);
			m_cameraComponent.reset();
			m_grid.reset();
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
				s_editorCamera->RenderCamera();
		}

		void EditorCamera::Update()
		{
			if (s_editorCamera)
				s_editorCamera->UpdateCamera();
		}

		void EditorCamera::SelectObject(object::GameObject * gameObject)
		{
			s_selectedObjects.clear();
			if (gameObject)
				s_selectedObjects.push_back(gameObject);
			GetEditorCamera()->m_hasSelectionChanged = true;
		}

		void EditorCamera::UnselectObject(GameObject * gameObject)
		{
			for (int i = 0; i < s_selectedObjects.size(); ++i)
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
			return m_cameraComponent.get();
		}

		void EditorCamera::SetManipulatorOperation(ImGuizmo::OPERATION operation)
		{
			s_editorCamera->m_manipulatorOperation = operation;
		}

		ImGuizmo::OPERATION EditorCamera::GetManipulatorOperation()
		{
			return s_editorCamera->m_manipulatorOperation;
		}

		void EditorCamera::ToggleManipulatorMode()
		{
			s_editorCamera->m_manipulatorMode = s_editorCamera->m_manipulatorMode == ImGuizmo::MODE::WORLD ? ImGuizmo::MODE::LOCAL : ImGuizmo::MODE::WORLD;
		}

		void EditorCamera::RenderCamera()
		{
			RenderSelectedObjects();
			m_cameraComponent->Render();
			RenderGizmos();
			Physics::DrawDebug(m_cameraComponent.get());
			m_grid->Draw(m_cameraComponent.get());
		}

		void EditorCamera::UpdateCamera()
		{
			Input::ResetScrollWheelValue();
			m_manipulatorSnapping = false;
			HWND focus = GetForegroundWindow();

			if (!Window::GetEditorWindow())
				return;

			//Toggle editor mode on scene camera
			if (Input::GetMouseButtonDown(Input::MouseButtons::RIGHT))
				Input::SetMouseMode(Input::MouseMode::POSITION_RELATIVE);

			if (Input::GetMouseButtonUp(Input::MouseButtons::RIGHT))
				Input::SetMouseMode(Input::MouseMode::POSITION_ABSOLUTE);

			//Scroll doesn't work for some reason... Commented out for now
			/*if (Input::GetMouseScrollWheel() > 0)
				m_transform->Translate(m_transform->Forward()*ThomasTime::GetActualDeltaTime() * 3000.f);

			if (Input::GetMouseScrollWheel() < 0)
				m_transform->Translate(-m_transform->Forward()*ThomasTime::GetActualDeltaTime() * 3000.f);*/

			if (Input::GetMouseButton(Input::MouseButtons::RIGHT))
			{
				float speed = m_speed;

				//Increase camera speed
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

				m_rotationX += Input::GetMouseX() * ThomasTime::GetActualDeltaTime() * m_sensitivity;
				m_rotationY += Input::GetMouseY() * ThomasTime::GetActualDeltaTime() * m_sensitivity;
				m_transform->SetRotation(-m_rotationX, -m_rotationY, 0);
			}
			else if (Input::GetMouseButtonDown(Input::MouseButtons::LEFT))
			{
				if (!ImGuizmo::IsOver())
				{
					object::GameObject* gObj = FindClickedGameObject();
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

		void EditorCamera::RenderSelectedObjects()
		{
			if (!m_objectHighlighter)
				return;
			for (object::GameObject* gameObject : s_selectedObjects)
			{
				if (!gameObject->GetActive())
					continue;

				object::component::RenderComponent* renderComponent = gameObject->GetComponent<object::component::RenderComponent>();
				if (renderComponent)
				{
					resource::Model* model = gameObject->GetComponent<object::component::RenderComponent>()->GetModel();
					if (model)
					{
						for (std::shared_ptr<graphics::Mesh> mesh : model->GetMeshes())
							graphics::Renderer::SubmitCommand(graphics::RenderCommand(gameObject->m_transform->GetWorldMatrix(), mesh, m_objectHighlighter.get(), m_cameraComponent.get()));
					}
				}
			}
		}

		void EditorCamera::RenderGizmos()
		{
			for (int i = 0; i < s_selectedObjects.size(); i++)
			{
				object::GameObject* gameObject = s_selectedObjects[i];
				ImGuiIO& io = ImGui::GetIO();
				ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
				math::Matrix worldMatrix = gameObject->m_transform->GetLocalWorldMatrix();

				float bounds[] = { 5,5,5 };
				float snap[] = { 1,1,1 };

				if (m_manipulatorOperation == ImGuizmo::OPERATION::ROTATE)
					snap[0] = 15;

				math::Matrix deltaMatrix;
				ImGuizmo::Manipulate(
					*(m_cameraComponent->GetViewMatrix() * math::Matrix::CreateScale(m_manipulatorScale)).m, *m_cameraComponent->GetProjMatrix().m,
					m_manipulatorOperation, m_manipulatorMode, *worldMatrix.m, *deltaMatrix.m, m_manipulatorSnapping ? snap : 0);

				if (worldMatrix != gameObject->m_transform->GetLocalWorldMatrix())
				{
					gameObject->m_transform->SetLocalMatrix(worldMatrix);
					gameObject->m_transform->SetDirty(true);
				}
			}
		}

		object::GameObject* EditorCamera::FindClickedGameObject()
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

