#include "Camera.h"
#include "../../Window.h"
#include "../GameObject.h"
#include "../../graphics/Skybox.h"
#include "../../graphics/TextRender.h"
#include "Transform.h"
#include "../../graphics/Renderer.h"
#include <algorithm>
#include "../../Input.h"
#include "../../editor/gizmos/Gizmos.h"
namespace thomas
{
	namespace object
	{
		namespace component
		{
			std::vector<Camera*> Camera::s_allCameras;
			void Camera::UpdateProjMatrix()
			{
				m_projMatrix = math::Matrix::CreatePerspectiveFieldOfView(math::DegreesToRadians(m_fov), GetViewport().AspectRatio(), m_near, m_far);
				m_frustrum = math::BoundingFrustum(m_projMatrix);
			}

			Camera::Camera(bool dontAddTolist)
			{
				m_fov = 70;
				m_near = 0.1;
				m_far = 10000;
				m_viewport = math::Viewport(0, 0, 1, 1);
				m_targetDisplay = -1;
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

			math::Ray Camera::ScreenPointToRay(math::Vector2 point)
			{
				// Move the mouse cursor coordinates into the -1 to +1 range.
				Window* window = Window::GetWindow(m_targetDisplay);
				float pointX = ((2.0f * (float)point.x) / (float)window->GetWidth()) - 1.0f;
				float pointY = (((2.0f * (float)point.y) / (float)window->GetHeight()) - 1.0f) * -1.0f;
				// Adjust the points using the projection matrix to account for the aspect ratio of the viewport.
				pointX /= m_projMatrix._11;
				pointY /= m_projMatrix.Transpose()._22;


				// Get the inverse of the view matrix.
				math::Matrix inverseViewMatrix = GetViewMatrix().Invert();

				math::Vector3 direction(pointX, pointY, -1.0f);

				// Calculate the direction of the picking ray in view space.
				direction = math::Vector3::TransformNormal(direction, inverseViewMatrix);

				// Get the origin of the picking ray which is the position of the camera
				math::Vector3 origin = GetPosition();
				direction.Normalize();
				return math::Ray(origin, direction);
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
				if (viewNear > 0)
					m_near = viewNear;
				UpdateProjMatrix();
			}

			void Camera::SetFar(float viewFar)
			{
				if (viewFar > 0)
					m_far = viewFar;
				UpdateProjMatrix();
			}

			math::Viewport Camera::GetViewport()
			{
				Window* window = Window::GetWindow(m_targetDisplay);
				if (window)
					return math::Viewport(m_viewport.x, m_viewport.y, m_viewport.width * window->GetWidth(), m_viewport.height * window->GetHeight());
				else
					return m_viewport;
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

			void Camera::OnDrawGizmos()
			{
				
			}

			void Camera::OnDrawGizmosSelected()
			{
				//editor::Gizmos::SetMatrix(m_gameObject->m_transform->GetWorldMatrix().Transpose());
				editor::Gizmos::SetColor(math::Color(0.6, 0.6, 0.6));
				editor::Gizmos::DrawFrustum(GetFrustrum());
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

			math::BoundingFrustum Camera::GetFrustrum()
			{
				math::BoundingFrustum frustrum;
				m_frustrum.Transform(frustrum, m_gameObject->m_transform->GetWorldMatrix());
				return frustrum;
			}

		}
	}
}