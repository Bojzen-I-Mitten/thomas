#pragma once
#include "Component.h"
#include "../../utils/Math.h"
/**
*Camera class
*/
namespace thomas
{
	namespace graphics { class Skybox; }
	namespace object
	{
		namespace component
		{
			class THOMAS_API Camera : public Component
			{
			private:
				void UpdateProjMatrix();

			public:
				static std::vector<Camera*> s_allCameras;

				Camera();
				Camera(bool dontAddTolist);
				~Camera();
				math::Matrix GetViewMatrix();
				math::Matrix GetProjMatrix();
				math::Matrix GetViewProjMatrix();
				math::Vector3 GetPosition();

				float GetFov();
				void SetFov(float fov);

				float GetNear();
				void SetNear(float viewNear);

				float GetFar();
				void SetFar(float viewFar);

				math::Viewport GetViewport();
				void SetViewport(math::Viewport viewport);
				void SetViewport(float x, float y, float width, float height);

				float GetAspectRatio();

				void Render();

				void SetTargetDisplay(int index);
				int GetTargetDisplayIndex();
				
			private:
				math::Matrix m_projMatrix;
				float m_fov;
				float m_near;
				float m_far;
				int m_targetDisplay;
				math::Viewport m_viewport;

			};
		}
	}
}