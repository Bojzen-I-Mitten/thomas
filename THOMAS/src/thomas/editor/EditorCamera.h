#pragma once
#include "../Common.h"
#include "../object/GameObject.h"
#include <vector>
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
		public:
			
			static void Init();
			static EditorCamera* GetEditorCamera();
			static void Render();
			static void Update();
			static void SelectObject(GameObject* gameObject);
			static void UnselectObject(GameObject* gameObject);
			static std::vector<object::GameObject*> GetSelectedObjects();
			static bool HasSelectionChanged();
			static void SetHasSelectionChanged(bool selectionChanged);
			object::component::Camera* GetCamera();
		private:
			float m_sensitivity = 1.0f;
			float rotationX = 0.0f;
			float rotationY = 0.0f;
			float m_speed;

			object::component::Camera* m_cameraComponent;
			graphics::Material* m_objectHighlighter;
			EditorGrid* m_grid;
			bool m_hasSelectionChanged;
			static EditorCamera* s_editorCamera;
			static std::vector<object::GameObject*> s_selectedObjects;
		};
	}
}
