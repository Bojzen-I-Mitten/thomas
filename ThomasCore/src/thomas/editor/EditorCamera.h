/*
	Singeleton class for management of the editor camera in the scene. 
 */

#pragma once
#include "..\Common.h"
#include "..\object\GameObject.h"
#include <imgui\ImGuizmo.h>
#include <vector>
#include <memory>

namespace thomas
{
	namespace resource
	{
		class Material;
	}

	namespace editor
	{
		class EditorGrid;

		class EditorCamera : public object::GameObject
		{
		public:		
			static void Init();
			static void Destroy();
			static void Render();
			static void Update();

		public:
			static bool HasSelectionChanged();
			static void ToggleManipulatorMode();
			static void SelectObject(GameObject* gameObject);
			static void UnselectObject(GameObject* gameObject);
			static void UnselectObjects();

		public:
			static void SetHasSelectionChanged(const bool & selectionChanged);
			static void SetManipulatorOperation(ImGuizmo::OPERATION operation);

		public:
			static EditorCamera* GetEditorCamera();
			static ImGuizmo::OPERATION GetManipulatorOperation();
			static std::vector<object::GameObject*> GetSelectedObjects();
			object::component::Camera* GetCamera() const;

		private:
			void RenderCamera();
			void UpdateCamera();
			void RenderSelectedObjects();
			void RenderGizmos();
			void MoveAndRotateCamera();
			object::GameObject* FindClickedGameObject();
			EditorCamera();
			~EditorCamera();

		private:
			ImGuizmo::OPERATION m_manipulatorOperation;
			ImGuizmo::MODE m_manipulatorMode;
			std::unique_ptr<object::component::Camera> m_cameraComponent;
			std::unique_ptr<resource::Material> m_objectHighlighter;
			std::unique_ptr<EditorGrid> m_grid;

		private:
			float m_sensitivity;
			float m_rotationX;
			float m_rotationY;
			float m_speed;
			float m_manipulatorScale;
			bool m_manipulatorSnapping;	
			bool m_hasSelectionChanged;

		private:
			static EditorCamera* s_editorCamera;
			static std::vector<object::GameObject*> s_selectedObjects;
		};
	}
}
