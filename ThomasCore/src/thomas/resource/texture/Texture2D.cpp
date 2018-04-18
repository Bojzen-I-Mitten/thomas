#include "Texture2D.h"
#include "../../utils/d3d.h"
namespace thomas
{
	namespace resource
	{
		Texture2D* Texture2D::s_blackTexture;
		Texture2D* Texture2D::s_whiteTexture;

		void Texture2D::Init()
		{
			DirectX::PackedVector::XMUBYTEN4 colors[16];
			std::fill(std::begin(colors), std::end(colors), math::Color(0, 0, 0, 1).RGBA());
			s_blackTexture = new Texture2D((void*)&colors, 4, 4, false, true);
			s_blackTexture->m_path = "Black Texture";
			std::fill(std::begin(colors), std::end(colors), math::Color(1, 1, 1, 1).RGBA());
			s_whiteTexture = new Texture2D((void*)&colors, 4, 4, false, true);
			s_whiteTexture->m_path = "White Texture";
			s_whiteTexture->GetPixels();
		}
		void Texture2D::Destroy()
		{
			delete s_blackTexture;
			delete s_whiteTexture;
		}

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
				data = new DirectX::ScratchImage();
			}
		}


		Texture2D::Texture2D(int width, int height, bool mipMap, bool linear) : Texture2D(nullptr, width, height, mipMap, linear)
		{
			
		}

		Texture2D::Texture2D(void * initData, int width, int height, bool mipMap, bool linear)
		{
			m_width = width;
			m_height = height;
			m_mipmapCount = 1;
			m_mipMap = mipMap;
			m_linear = linear;

			ID3D11Texture2D *textureInterface = nullptr;
			utils::D3d::CreateTexture(initData, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, textureInterface, m_srv, mipMap, 1);
			m_resource = textureInterface;
			data = new DirectX::ScratchImage();
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

		std::vector<math::Color> Texture2D::GetPixels()
		{
			
			HRESULT hr = DirectX::CaptureTexture(ThomasCore::GetDevice(), ThomasCore::GetDeviceContext(), m_resource, *data);

			std::vector<math::Color> pixels;
			DirectX::PackedVector::XMUBYTEN4* rawPixels = (DirectX::PackedVector::XMUBYTEN4*)data->GetPixels();

			for (int i = 0; i < data->GetPixelsSize() / 4; i++)
			{
				pixels.push_back(math::Color(rawPixels[i].v));
			}
			return pixels;
			data->Release();
		}

		byte * Texture2D::GetRawBGRAPixels()
		{
			DirectX::ScratchImage firstData;
			HRESULT hr = DirectX::CaptureTexture(ThomasCore::GetDevice(), ThomasCore::GetDeviceContext(), m_resource, firstData);
			hr = DirectX::Convert(*firstData.GetImage(0, 0, 0), DXGI_FORMAT_B8G8R8A8_UNORM, DirectX::TEX_FILTER_DEFAULT, DirectX::TEX_THRESHOLD_DEFAULT, *data);
			firstData.Release();

			return data->GetPixels();
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
