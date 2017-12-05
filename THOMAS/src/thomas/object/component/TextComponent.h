#pragma once
#include "Component.h"
#include "../../utils/Math.h"

namespace thomas
{
	namespace object
	{
		namespace component
		{
			class TextComponent : public Component
			{
			public:
				TextComponent();
				~TextComponent();
				math::Vector2 GetPosition();
				math::Vector3 GetColor();
				std::string GetFont();
				std::string GetOutput();
				float GetRotation();
				float GetScale();
				bool GetDropshadow();
				bool GetOutline();
				bool GetOrigin();

				void SetFont(std::string font);
				void SetPositionX(float posX);
				void SetPositionY(float posY);
				void SetRotation(float rotation);
				void SetScale(float scale);
				void SetColor(math::Vector3 color);
				void SetDropshadow(bool dropshadow);
				void SetOutline(bool outline);
				void SetOutput(std::string output);
				void SetOrigin(bool origin);
	
			private:
				std::string m_font;
				std::string m_output;
				math::Vector2 m_fontPos;
				math::Vector3 m_color;
				float m_rotation;
				float m_scale;
				bool m_dropshadow;
				bool m_outline;
				bool m_origin;
			};
		}
	}
}