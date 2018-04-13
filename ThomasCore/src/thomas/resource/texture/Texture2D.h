#pragma once

#include "Texture.h"

namespace thomas
{
	namespace resource
	{
		class THOMAS_API Texture2D : public Texture
		{
		private:
			void LoadTextureFromFile(std::string path);
		public:
			Texture2D(int width, int height, bool mipMap, bool linear);
			Texture2D(std::string path);

			void OnChanged();

			static Texture2D* GetBlackTexture();
			static Texture2D* GetWhiteTexture();
		private:
			int m_mipmapCount;
			bool m_linear;
			bool m_mipMap;
			static Texture2D* s_blackTexture;
			static Texture2D* s_whiteTexture;
		};
	}
}