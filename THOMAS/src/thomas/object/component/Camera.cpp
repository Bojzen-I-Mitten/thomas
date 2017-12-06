#include "Camera.h"
#include "../../Window.h"
#include "../GameObject.h"
#include "../../graphics/Skybox.h"
#include "../../graphics/TextRender.h"
#include "Transform.h"
#include "../../graphics/Renderer.h"
namespace thomas
{
	namespace object
	{
		namespace component
		{
			void Camera::UpdateProjMatrix()
			{
				m_projMatrix = math::Matrix::CreatePerspectiveFieldOfView(math::DegreesToRadians(m_fov), m_viewport.AspectRatio() * Window::GetWindow(m_windowIndex)->GetRealAspectRatio(), m_near, m_far);
			}

			Camera::Camera()
			{
				m_windowIndex = 0; //TODO
				m_fov = 70;
				m_near = 0.5;
				m_far = 10000;
				m_viewport = math::Viewport(0, 0, 1,1);
				UpdateProjMatrix();
			}
			Camera::~Camera()
			{
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
				Window* window = Window::GetWindow(m_windowIndex);
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
				ThomasCore::GetDeviceContext()->RSSetViewports(1, GetViewport().Get11());
				graphics::Renderer::RenderCamera(this);
			}


		}
	}
}