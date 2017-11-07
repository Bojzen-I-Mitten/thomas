#include "TextRender.h"
#include "../ThomasCore.h"
#include "DirectXTK/SpriteBatch.h"
#include "DirectXTK/SpriteFont.h"
#include "DirectXTK/CommonStates.h"
#include "DirectXTK/Effects.h"
#include "../object/component/TextComponent.h"
namespace thomas
{
	namespace graphics
	{
		std::map<std::string, std::unique_ptr<DirectX::SpriteFont>> TextRender::s_fonts;
		math::Vector2 TextRender::s_fontPos;
		std::unique_ptr<DirectX::SpriteBatch> TextRender::s_spriteBatch;
		std::unique_ptr<DirectX::CommonStates> TextRender::s_states;
		std::unique_ptr<DirectX::BasicEffect> TextRender::s_effects;
		math::Vector2 TextRender::s_textOrigin;

		void TextRender::RenderText(std::string name, std::string output, float posX, float posY, float scale, float rotation,
									math::Vector3 color, bool dropShadow, bool outline, bool origin)
		{		
			if (s_fonts[name])
			{
				ThomasCore::GetDeviceContext()->OMSetBlendState(s_states->Opaque(), DirectX::Colors::Black, 0xFFFFFFFF);
	
				std::wstring holder = std::wstring(output.begin(), output.end());
				const wchar_t* result = holder.c_str();

				s_spriteBatch->Begin();


				if (origin)
				{
					s_textOrigin = s_fonts[name]->MeasureString(result);
					s_textOrigin.operator/=(2.f);
				}
				else
				{
					s_textOrigin = math::Vector2(0.0f, 0.0f);
				}

				if (dropShadow)
				{
					s_fonts[name]->DrawString(s_spriteBatch.get(), result,
						math::Vector2(posX, posY) + math::Vector2(1.f, 1.f), DirectX::Colors::Black, rotation, s_textOrigin, scale);
					s_fonts[name]->DrawString(s_spriteBatch.get(), result,
						math::Vector2(posX, posY) + math::Vector2(-1.f, 1.f), DirectX::Colors::Black, rotation, s_textOrigin, scale);
				}

				if (outline)
				{
					s_fonts[name]->DrawString(s_spriteBatch.get(), result,
						math::Vector2(posX, posY) + math::Vector2(-1.f, -1.f), DirectX::Colors::Black, rotation, s_textOrigin, scale);
					s_fonts[name]->DrawString(s_spriteBatch.get(), result,
						math::Vector2(posX, posY) + math::Vector2(1.f, -1.f), DirectX::Colors::Black, rotation, s_textOrigin, scale);
				}

				s_fonts[name]->DrawString(s_spriteBatch.get(), result, math::Vector2(posX, posY), color, rotation, s_textOrigin, scale);

				s_spriteBatch->End();

				//ThomasCore::GetDeviceContext()->OMSetDepthStencilState(NULL, 0);
				ThomasCore::GetDeviceContext()->OMSetBlendState(NULL, DirectX::Colors::Black, 0xFFFFFFFF);
			}
		}

		bool TextRender::LoadFont(std::string name, std::string font)
		{
			std::wstring holder = std::wstring(font.begin(), font.end());
			const wchar_t* result = holder.c_str();

			s_fonts[name] = std::make_unique<DirectX::SpriteFont>(ThomasCore::GetDevice(), result);

			if (!s_fonts[name])
			{
				LOG("Failed to load font for " << name);
				return false;
			}
			return true;
		}

		bool TextRender::Initialize()
		{
			s_spriteBatch = std::make_unique<DirectX::SpriteBatch>(ThomasCore::GetDeviceContext());
			s_states = std::make_unique<DirectX::CommonStates>(ThomasCore::GetDevice());
			s_effects = std::make_unique<DirectX::BasicEffect>(ThomasCore::GetDevice());

			if (!s_spriteBatch)
			{
				LOG("Failed to initialize spriteBatch");
				return false;
			}
			return true;
		}

		void TextRender::Destroy()
		{
			std::map<std::string, std::unique_ptr<DirectX::SpriteFont>>::iterator itr;
			itr = s_fonts.begin();

			while (itr != s_fonts.end())
			{
				itr = s_fonts.erase(itr);
			}
			
			s_spriteBatch.release();
			s_spriteBatch.reset();
			s_states.release();
			s_states.reset();
			s_effects.release();
			s_effects.reset();
		}

		void TextRender::RenderText(object::component::TextComponent* text)
		{
			RenderText(text->GetFont(), text->GetOutput(), text->GetPosition().x, text->GetPosition().y, text->GetScale(),
				text->GetRotation(), text->GetColor(), text->GetDropshadow(), text->GetOutline(), text->GetOrigin());
		}
	}
}