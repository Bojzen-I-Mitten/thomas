#pragma once
#include "../Common.h"
#include "../object/GameObject.h"
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
			EditorCamera();
		public:
			
			static void Init();
			static EditorCamera* GetEditorCamera();
			static void Render();
			static void Update();
			static void SelectObject(GameObject* gameObject);

		private:
			float m_sensitivity = 1.0f;
			float rotationX = 0.0f;
			float rotationY = 0.0f;

			object::component::Camera* m_cameraComponent;
			graphics::Material* m_objectHighlighter;
			EditorGrid* m_grid;
			static EditorCamera* s_editorCamera;
			static std::vector<object::GameObject*> s_selectedObjects;
		};
	}
}
