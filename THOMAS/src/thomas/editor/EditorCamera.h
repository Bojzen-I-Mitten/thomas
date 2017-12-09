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
		public:
			
			static void Init();
			static EditorCamera* GetEditorCamera();
			static void Render();
			static void Update();

		private:
			float m_sensitivity = 1.0f;
			float rotationX = 0.0f;
			float rotationY = 0.0f;
			void renderCamera();
			void updateCamera();
			EditorCamera();
			object::component::Camera* m_cameraComponent;
			EditorGrid* m_grid;
			static EditorCamera* s_editorCamera;

		};
	}
}
