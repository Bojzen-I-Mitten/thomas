#pragma once

#include "Texture.h"
#include "../../utils/Math.h"
#include <DirectXTex.h>
namespace thomas
{
	namespace resource
	{
		class THOMAS_API Texture2D : public Texture
		{
		private:
			void LoadTextureFromFile(std::string path);
		public:
			static void Init();
			static void Destroy();
			Texture2D(int width, int height, bool mipMap, bool linear);
			Texture2D(void* initData, int width, int height, bool mipMap, bool linear);
			Texture2D(std::string path);

			void OnChanged();
			std::vector<math::Color> GetPixels();
			byte* GetRawBGRAPixels();

			static Texture2D* GetBlackTexture();
			static Texture2D* GetWhiteTexture();
		private:
			DirectX::ScratchImage* data = nullptr;
			int m_mipmapCount;
			bool m_linear;
			bool m_mipMap;
			static Texture2D* s_blackTexture;
			static Texture2D* s_whiteTexture;
		};
	}
}