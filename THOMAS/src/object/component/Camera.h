#pragma once
#include "Component.h"
#include "../../utils/Math.h"
#include "../../graphics/Skybox.h"
#include "../../graphics/TextRender.h"
/**
*Camera class
*/
namespace thomas
{
	namespace object
	{
		namespace component
		{
			class THOMAS_API Camera : public Component
			{
			private:
				void UpdateProjMatrix();

			public:
				Camera();
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

				void SetSkybox(std::string path, std::string shaderName, int slot);

				void AddSkybox(std::string path, int slot);
				void SetSkyboxLerpValue(math::Vector3 lerp);

				void BindSkybox();
				void BindReflection();
				void UnbindSkybox();

				void ReleaseGUI();
				void ReleaseSkybox();
				
			private:
				math::Matrix m_projMatrix;
				float m_fov;
				float m_near;
				float m_far;
				math::Viewport m_viewport;
				graphics::Skybox* m_skybox;

			};
		}
	}
}