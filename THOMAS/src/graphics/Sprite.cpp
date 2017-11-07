#include "Sprite.h"
#include "../ThomasCore.h"
#include "DirectXTK/SpriteBatch.h"
#include "DirectXTK/WICTextureLoader.h"
#include "DirectXTK/CommonStates.h"
#include "../object/component/SpriteComponent.h"
namespace thomas
{
	namespace graphics
	{
		std::map<std::string, Sprite::SpriteData> Sprite::s_texture;
		std::unique_ptr<DirectX::SpriteBatch> Sprite::s_spriteBatch;
		DirectX::SimpleMath::Vector2 Sprite::s_screenPos;
		DirectX::SimpleMath::Vector2 Sprite::s_origin;
		std::unique_ptr<DirectX::CommonStates> Sprite::s_states;

		bool Sprite::LoadTexture(std::string name, std::string texture)
		{
			std::wstring holder = std::wstring(texture.begin(), texture.end());
			const wchar_t* result = holder.c_str();

			Microsoft::WRL::ComPtr<ID3D11Resource> resource;
			HRESULT hr = DirectX::CreateWICTextureFromFile(ThomasCore::GetDevice(), result, resource.GetAddressOf(), s_texture[name].srv.ReleaseAndGetAddressOf());

			if (FAILED(hr))
			{
				LOG("Failed to load " << result << " for GUI");
				return false;
			}

			Microsoft::WRL::ComPtr<ID3D11Texture2D> image;
			resource.As(&image);

			CD3D11_TEXTURE2D_DESC imageDesc;
			image->GetDesc(&imageDesc);
			
			s_texture[name].width = imageDesc.Width;
			s_texture[name].height = imageDesc.Height;

			s_origin.x = 0;
			s_origin.y = 0;

			return true;
		}

		void Sprite::Destroy()
		{
			std::map<std::string, Sprite::SpriteData>::iterator itr;
			itr = s_texture.begin();

			while (itr != s_texture.end())
			{
				itr = s_texture.erase(itr);
			}
			s_spriteBatch.release();
			s_spriteBatch.reset();
			s_states.release();
			s_states.reset();
		}

		bool Sprite::Initialize()
		{
			s_spriteBatch = std::make_unique<DirectX::SpriteBatch>(ThomasCore::GetDeviceContext());
			s_states = std::make_unique<DirectX::CommonStates>(ThomasCore::GetDevice());

			if (!s_spriteBatch)
			{
				LOG("Failed to initialize spriteBatch");
				return false;
			}
			return true;
		}

		UINT Sprite::GetImageWidth(object::component::SpriteComponent * sprite)
		{
			return s_texture[sprite->GetSignature()].width;
		}

		UINT Sprite::GetImageHeight(object::component::SpriteComponent * sprite)
		{
			return s_texture[sprite->GetSignature()].height;
		}


		void Sprite::RenderImage(std::string name, math::Vector4 color, float posX, float posY, math::Vector2 scale)
		{

			if(s_texture[name].srv)
			{
				s_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, s_states->NonPremultiplied());

				s_spriteBatch->Draw(s_texture[name].srv.Get(), math::Vector2(posX, posY), nullptr, color,
					0.f, s_origin, scale);

				s_spriteBatch->End();
			}
		}

		void Sprite::RenderImage(object::component::SpriteComponent * sprite)
		{
			RenderImage(sprite->GetSignature(), sprite->GetColor(), sprite->GetPosition().x, sprite->GetPosition().y, sprite->GetScale());	
		}
	}
}
