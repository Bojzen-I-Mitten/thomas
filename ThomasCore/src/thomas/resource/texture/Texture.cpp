#include "Texture.h"

namespace thomas
{
	namespace resource
	{
		void Texture::OnChanged()
		{
		}
		Texture::~Texture()
		{
			SAFE_RELEASE(m_srv);
			SAFE_RELEASE(m_resource);
		}
		Texture::WrapMode Texture::GetWrapMode()
		{
			return m_wrapMode;
		}
		void Texture::SetWrapMode(WrapMode mode)
		{
			m_wrapMode = mode;
		}
		Texture::Dimension Texture::GetDimension()
		{
			return m_texDim;
		}
		Texture::FilterMode Texture::GetFilterMode()
		{
			return m_filterMode;
		}
		void Texture::SetFilterMode(FilterMode mode)
		{
			m_filterMode = mode;
		}
		int Texture::GetAnistropicLevel()
		{
			return m_anistropicFilteringLevel;
		}
		void Texture::SetAnistropicLevel(int value)
		{
			m_anistropicFilteringLevel = value;
		}
		int Texture::GetWidth()
		{
			return m_width;
		}
		int Texture::GetHeight()
		{
			return m_height;
		}
		ID3D11ShaderResourceView * Texture::GetResourceView()
		{
			return m_srv;
		}
	}
}