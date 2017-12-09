#include "Camera.h"
#include "../../Window.h"
#include "../GameObject.h"
#include "../../graphics/Skybox.h"
#include "../../graphics/TextRender.h"
#include "Transform.h"
#include "../../graphics/Renderer.h"
#include <algorithm>
namespace thomas
{
	namespace object
	{
		namespace component
		{
			std::vector<Camera*> Camera::s_allCameras;
			void Camera::UpdateProjMatrix()
			{
				m_projMatrix = math::Matrix::CreatePerspectiveFieldOfView(math::DegreesToRadians(m_fov), m_viewport.AspectRatio() * Window::GetWindow(m_targetDisplay)->GetRealAspectRatio(), m_near, m_far);
			}

			Camera::Camera(bool dontAddTolist)
			{
				m_fov = 70;
				m_near = 0.1;
				m_far = 10000;
				m_viewport = math::Viewport(0, 0, 1, 1);
				m_targetDisplay = 0;
				UpdateProjMatrix();
			}

			Camera::Camera()
			{
				m_fov = 70;
				m_near = 0.5;
				m_far = 10000;
				m_viewport = math::Viewport(0, 0, 1,1);
				m_targetDisplay = 0;
				UpdateProjMatrix();
				s_allCameras.push_back(this);
				std::sort(s_allCameras.begin(), s_allCameras.end(), [](Camera* a, Camera* b) 
				{
					return a->GetTargetDisplayIndex() < b->GetTargetDisplayIndex();
				});
			}
			Camera::~Camera()
			{
				for (int i = 0; i < s_allCameras.size(); i++)
				{
					if (s_allCameras[i] == this)
					{
						s_allCameras.erase(s_allCameras.begin() + i);
						break;
					}
						
				}
			}

			math::Matrix Camera::GetProjMatrix()
			{
				return m_projMatrix;
			}

			math::Matrix Camera::GetViewMatrix()
			{
				math::Matrix viewMatrix =  m_gameObject->m_transform->GetWorldMatrix();
				return viewMatrix.Invert();
			}

			math::Matrix Camera::GetViewProjMatrix()
			{
				return GetViewMatrix() * m_projMatrix;
			}

			math::Vector3 Camera::GetPosition()
			{
				return m_gameObject->m_transform->GetPosition();
			}

			float Camera::GetFov()
			{
				return m_fov;
			}

			float Camera::GetNear()
			{
				return m_near;
			}

			float Camera::GetFar()
			{
				return m_far;
			}

			void Camera::SetFov(float fov)
			{
				m_fov = fov;
				UpdateProjMatrix();
			}

			void Camera::SetNear(float viewNear)
			{
				m_near = viewNear;
				UpdateProjMatrix();
			}

			void Camera::SetFar(float viewFar)
			{
				m_far = viewFar;
				UpdateProjMatrix();
			}

			math::Viewport Camera::GetViewport()
			{
				UpdateProjMatrix();
				Window* window = Window::GetWindow(m_targetDisplay);
				return math::Viewport(m_viewport.x, m_viewport.y, m_viewport.width * window->GetWidth(), m_viewport.height * window->GetHeight());
			}

			void Camera::SetViewport(math::Viewport viewport)
			{
				m_viewport = viewport;
				UpdateProjMatrix();
			}

			void Camera::SetViewport(float x, float y, float width, float height)
			{
				SetViewport(math::Viewport(x, y, width, height));
			}

			float Camera::GetAspectRatio()
			{
				return m_viewport.AspectRatio();
			}

			void Camera::Render()
			{
				graphics::Renderer::BindCamera(this);
				graphics::Renderer::Render();
			}

			void Camera::SetTargetDisplay(int index)
			{
				if (Window::GetWindows().size() < index)
				{
					m_targetDisplay = index;
					UpdateProjMatrix();
				}
					
				
			}

			int Camera::GetTargetDisplayIndex()
			{
				return m_targetDisplay;
			}

		}
	}
}