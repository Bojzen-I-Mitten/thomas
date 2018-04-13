#include "Texture2D.h"
#include "../../utils/d3d.h"
namespace thomas
{
	namespace resource
	{
		Texture2D* Texture2D::s_blackTexture;
		Texture2D* Texture2D::s_whiteTexture;
		void Texture2D::LoadTextureFromFile(std::string path)
		{
			if (utils::D3d::LoadTextureFromFile(path, m_resource, m_srv))
			{
				ID3D11Texture2D *textureInterface;

				m_resource->QueryInterface<ID3D11Texture2D>(&textureInterface);

				D3D11_TEXTURE2D_DESC desc;

				textureInterface->GetDesc(&desc);
				m_mipmapCount = desc.MipLevels;
				m_width = desc.Width;
				m_height = desc.Height;

				textureInterface->Release();
			}
		}
		Texture2D::Texture2D(int width, int height, bool mipMap, bool linear)
		{
			m_width = width;
			m_height = height;
			m_mipmapCount = 1;
			m_mipMap = mipMap;
			m_linear = linear;

			ID3D11Texture2D *textureInterface;
			utils::D3d::CreateTexture(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT, textureInterface, m_srv, mipMap, 1);
			m_resource = textureInterface;
		}

		Texture2D::Texture2D(std::string path) : Texture(path)
		{
			LoadTextureFromFile(path);
		}

		void Texture2D::OnChanged()
		{
			SAFE_RELEASE(m_srv);
			SAFE_RELEASE(m_resource);
			LoadTextureFromFile(m_path);
		}

		Texture2D * Texture2D::GetBlackTexture()
		{
			return s_blackTexture;
		}

		Texture2D * Texture2D::GetWhiteTexture()
		{
			return s_whiteTexture;
		}

	}
}
