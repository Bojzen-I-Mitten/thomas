#include "SpriteComponent.h"
#include "../../Window.h"
#include "../../graphics/Sprite.h"

namespace thomas
{
	namespace object
	{
		namespace component
		{
			void SpriteComponent::OnHover()
			{
				m_currentColor = m_hoverColor;
				m_hovering = true;
			}
			SpriteComponent::SpriteComponent() : Component("SpriteComponent")
			{
				m_interactable = false;
				m_hovering = false;
				m_currentColor = math::Color(1, 1, 1, 1);
				m_scale = math::Vector2(1, 1);
			}

			math::Vector2 SpriteComponent::GetPosition()
			{
				return m_imagePos;
			}

			math::Vector2 SpriteComponent::GetScale()
			{
				return m_scale;
			}

			UINT SpriteComponent::GetWidth()
			{
				return thomas::graphics::Sprite::GetImageWidth(this);
			}

			UINT SpriteComponent::GetHeight()
			{
				return thomas::graphics::Sprite::GetImageHeight(this);
			}

			std::string SpriteComponent::GetSignature()
			{
				return m_name;
			}

			math::Vector4 SpriteComponent::GetColor()
			{
				return m_currentColor;
			}

			bool SpriteComponent::isHovering()
			{
				return m_hovering;
			}

			void SpriteComponent::SetHovering(bool set)
			{
				if (set)
				{
					OnHover();
					m_interactable = false;
				}
				else
				{
					m_currentColor = m_baseColor;
					m_hovering = false;
					m_interactable = true;
				}
			}

			void SpriteComponent::SetName(std::string name)
			{
				m_name = name;
			}

			void SpriteComponent::SetPositionX(float posX)
			{
				m_imagePos.x = posX;
			}

			void SpriteComponent::SetPositionY(float posY)
			{
				m_imagePos.y = posY;
			}

			void SpriteComponent::SetScale(math::Vector2 scale)
			{
				m_scale = scale;
			}

			void SpriteComponent::SetColor(math::Vector4 color)
			{
				m_baseColor = color;
				m_currentColor = m_baseColor;
			}

			void SpriteComponent::SetHoverColor(math::Color color)
			{
				m_hoverColor = color;
			}

			void SpriteComponent::SetInteractable(bool interactable)
			{
				m_interactable = interactable;
			}

			void SpriteComponent::Update()
			{
				
		
				if (m_interactable)
				{
					m_currentColor = m_baseColor;
					m_hovering = false;
					//Input::SetMouseMode(Input::MouseMode::POSITION_ABSOLUTE);
					//math::Vector2 mousePos = Input::GetMousePosition();

					////Construct boundaries
					//float xLeft = GetPosition().x;
					//float xRight = GetPosition().x + GetWidth();
					//float yTop = GetPosition().y;
					//float yDown = GetPosition().y + GetHeight();

					//if (mousePos.x >= xLeft && mousePos.x <= xRight && mousePos.y <= yDown && mousePos.y >= yTop)
					//{
					//	OnHover();
					//}
					//else
					//{
					//	m_currentColor = m_baseColor;
					//}
				}
			}
		}
	}
}
