#pragma once
#include "../utils/Math.h"
#include "../ThomasCore.h"
#include <wrl.h>
#include "../../../DirectXTK-dec2016/Inc/SpriteBatch.h"
#include "../../../DirectXTK-dec2016/Inc/WICTextureLoader.h"
#include "../object/component/SpriteComponent.h"
#include <map>
#include "../../../DirectXTK-dec2016/Inc/CommonStates.h"

namespace thomas
{
	namespace graphics
	{
		class Sprite
		{
			struct SpriteData
			{
				UINT width;
				UINT height;
				Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
			};

		public:
			static bool LoadTexture(std::string name, std::string texture);
			static bool Initialize();
			static UINT GetImageWidth(object::component::SpriteComponent * sprite);
			static UINT GetImageHeight(object::component::SpriteComponent * sprite);
			static void Destroy();
			static void RenderImage(std::string name, math::Vector4 color, float posX, float posY, math::Vector2 scale);
			static void RenderImage(object::component::SpriteComponent* sprite);

		private:
			static std::map<std::string, SpriteData> s_texture;
			static std::unique_ptr<DirectX::SpriteBatch> s_spriteBatch;
			static DirectX::SimpleMath::Vector2 s_screenPos;
			static DirectX::SimpleMath::Vector2 s_origin;
			static std::unique_ptr<DirectX::CommonStates> s_states;
		};
	}
}

