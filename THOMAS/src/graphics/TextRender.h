#pragma once
#include "../utils/Math.h"

#include <map>
#include <memory>

namespace DirectX {
	class SpriteBatch;
	class CommonStates;
	class SpriteFont;
	class BasicEffect;
}

namespace thomas
{
	namespace object { namespace component { class TextComponent; } }
	namespace graphics
	{
		class TextRender
		{
		public:
			static void Destroy();
			static void RenderText(object::component::TextComponent* text);
			static void RenderText(std::string name, std::string output, float posX, float posY, float scale, float rotation,
								   math::Vector3 color, bool dropShadow, bool outline, bool origin);
			static bool LoadFont(std::string name, std::string font);
			static bool Initialize();

		private:
			static std::map<std::string, std::unique_ptr<DirectX::SpriteFont>> s_fonts;
			static math::Vector2 s_fontPos;
			static std::unique_ptr<DirectX::SpriteBatch> s_spriteBatch;
			static std::unique_ptr<DirectX::CommonStates> s_states;
			static std::unique_ptr<DirectX::BasicEffect> s_effects;
			static math::Vector2 s_textOrigin;
		};
	}
}

