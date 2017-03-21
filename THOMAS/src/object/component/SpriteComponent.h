#pragma once
#include "Component.h"
#include "../../utils/Math.h"

namespace thomas
{
	namespace object
	{
		namespace component
		{
			class SpriteComponent : public Component
			{
			private:
				void OnHover();
			public:
				SpriteComponent();
				math::Vector2 GetPosition();
				math::Vector2 GetScale();
				UINT GetWidth();
				UINT GetHeight();
				std::string GetSignature();
				math::Vector4 GetColor();
				bool isHovering();

				void SetHovering(bool set);
				void SetName(std::string name);
				void SetPositionX(float posX);
				void SetPositionY(float posY);
				void SetScale(math::Vector2 scale);
				void SetColor(math::Vector4 color);
				void SetHoverColor(math::Color color);
				void SetInteractable(bool interactable);
				void Update();
				
			private:
				math::Vector2 m_imagePos;
				math::Vector2 m_scale;
				math::Color m_baseColor;
				math::Color m_currentColor;
				math::Color m_hoverColor;
				std::string m_name;
				bool m_hovering;
				bool m_interactable;

			};
		}
	}
}

