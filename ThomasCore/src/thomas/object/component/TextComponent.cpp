#include "TextComponent.h"

namespace thomas
{
	namespace object
	{
		namespace component
		{
			TextComponent::TextComponent()
			{
				
			}

			TextComponent::~TextComponent()
			{
			}

			math::Vector2 TextComponent::GetPosition()
			{
				return m_fontPos;
			}

			math::Vector3 TextComponent::GetColor()
			{
				return m_color;
			}

			std::string TextComponent::GetFont()
			{
				return m_font;
			}

			std::string TextComponent::GetOutput()
			{
				return m_output;
			}

			float TextComponent::GetRotation()
			{
				return m_rotation;
			}

			float TextComponent::GetScale()
			{
				return m_scale;
			}

			bool TextComponent::GetDropshadow()
			{
				return m_dropshadow;
			}

			bool TextComponent::GetOutline()
			{
				return m_outline;
			}

			bool TextComponent::GetOrigin()
			{
				return m_origin;
			}

			void TextComponent::SetFont(std::string font)
			{
				m_font = font;
			}

			void TextComponent::SetPositionX(float posX)
			{
				m_fontPos.x = posX;
			}

			void TextComponent::SetPositionY(float posY)
			{
				m_fontPos.y = posY;
			}

			void TextComponent::SetRotation(float rotation)
			{
				m_rotation = rotation;
			}

			void TextComponent::SetScale(float scale)
			{
				m_scale = scale;
			}

			void TextComponent::SetColor(math::Vector3 color)
			{
				m_color = color;
			}

			void TextComponent::SetDropshadow(bool dropshadow)
			{
				m_dropshadow = dropshadow;
			}

			void TextComponent::SetOutline(bool outline)
			{
				m_outline = outline;
			}

			void TextComponent::SetOutput(std::string output)
			{
				m_output = output;
			}

			void TextComponent::SetOrigin(bool origin)
			{
				m_origin = origin;
			}

		}
	}
}