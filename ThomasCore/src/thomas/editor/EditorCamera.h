#pragma once
#include "../Common.h"
#include "../object/GameObject.h"
#include <vector>
#include <imgui\ImGuizmo.h>
namespace thomas
{
	namespace graphics
	{
		class Material;
	}
	namespace editor
	{
		class EditorGrid;
		class THOMAS_API EditorCamera : public object::GameObject
		{
		private:
			
			void renderCamera();
			void updateCamera();
			void renderSelectedObjects();
			void renderGizmos();
			object::GameObject* findClickedGameObject();
			EditorCamera();
			~EditorCamera();
		public:
			
			static void Destroy();
			static void Init();
			static EditorCamera* GetEditorCamera();
			static void Render();
			static void Update();
			static void SelectObject(GameObject* gameObject);
			static void UnselectObject(GameObject* gameObject);
			static void UnselectObjects();
			static std::vector<object::GameObject*> GetSelectedObjects();
			static bool HasSelectionChanged();
			static void SetHasSelectionChanged(bool selectionChanged);
			object::component::Camera* GetCamera();

			static void SetManipulatorOperation(ImGuizmo::OPERATION operation);
			static ImGuizmo::OPERATION GetManipulatorOperation();

			static void ToggleManipulatorMode();
		private:
			ImGuizmo::OPERATION m_manipulatorOperation;
			ImGuizmo::MODE m_manipulatorMode;

			float m_sensitivity = 1.0f;
			float rotationX;
			float rotationY;
			float m_speed;
			float m_manipulatorScale = 2.0f;
			bool m_manipulatorSnapping = false;

			object::component::Camera* m_cameraComponent;
			graphics::Material* m_objectHighlighter;
			EditorGrid* m_grid;
			bool m_hasSelectionChanged;
			static EditorCamera* s_editorCamera;
			static std::vector<object::GameObject*> s_selectedObjects;
		};
	}
}
