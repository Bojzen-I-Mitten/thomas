#include "Texture.h"
#include "Shader.h"
#include "../ThomasCore.h"
#include  "../utils/AssimpLoader.h"
#include "../utils/d3d.h"
#include <assimp\material.h>
namespace thomas
{
	namespace graphics
	{
		std::vector<Texture*> Texture::s_loadedTextures;
		Texture::SamplerStates Texture::s_samplerStates;
		void Texture::ReleaseSamplers()
		{
			s_samplerStates.CLAMP->Release();
			s_samplerStates.WRAP->Release();
			s_samplerStates.DECAL->Release();
			s_samplerStates.MIRROR->Release();
		}
		bool Texture::Init()
		{
			LOG("Creating Texture samplers");
			return CreateTextureSamplers();
		}
		Texture* Texture::CreateTexture(int mappingMode, TextureType type, std::string path)
		{
			for (unsigned int i = 0; i < s_loadedTextures.size(); ++i)
			{
				if (s_loadedTextures[i]->GetName() == path)
					return s_loadedTextures[i];
			}

			Texture* texture = new Texture(mappingMode, type, path);
			if (texture)
				s_loadedTextures.push_back(texture);
			return texture;
		}

		Texture * Texture::CreateTexture(SamplerState samplerState, TextureType type, std::string path)
		{
			for (unsigned int i = 0; i < s_loadedTextures.size(); ++i)
			{
				if (s_loadedTextures[i]->GetName() == path && s_loadedTextures[i]->GetTextureType() == type)
					return s_loadedTextures[i];
			}

			Texture* texture = new Texture(samplerState, type, path);
			if (texture)
				s_loadedTextures.push_back(texture);
			return texture;
		}

		Texture * Texture::CreateTexture(SamplerState samplerState, int slot, std::string name, ID3D11ShaderResourceView * textureView)
		{
			for (unsigned int i = 0; i < s_loadedTextures.size(); ++i)
			{
				if (s_loadedTextures[i]->GetName() == name && s_loadedTextures[i]->GetTextureType() == TextureType::UNDEFINED)
					return s_loadedTextures[i];
			}

			Texture* texture = new Texture(samplerState, slot, name, textureView);
			if (texture)
				s_loadedTextures.push_back(texture);
			return texture;
		}

		Texture * Texture::CreateTexture(SamplerState samplerState, int slot, std::string path)
		{
			for (unsigned int i = 0; i < s_loadedTextures.size(); ++i)
			{
				if (s_loadedTextures[i]->GetName() == path && s_loadedTextures[i]->GetTextureType() == TextureType::UNDEFINED)
					return s_loadedTextures[i];
			}

			Texture* texture = new Texture(samplerState, slot, path);
			if (texture)
				s_loadedTextures.push_back(texture);
			return texture;
		}

		Texture * Texture::CreateTexture(SamplerState samplerState, int slot, TextureType type, std::string path)
		{
			for (unsigned int i = 0; i < s_loadedTextures.size(); ++i)
			{
				if (s_loadedTextures[i]->GetName() == path && s_loadedTextures[i]->GetTextureType() == type)
					return s_loadedTextures[i];
			}

			Texture* texture = new Texture(samplerState, slot, type, path);
			if (texture)
				s_loadedTextures.push_back(texture);
			return texture;
		}

		ID3D11SamplerState * Texture::GetSamplerState()
		{
			return m_samplerState;
		}

		ID3D11SamplerState * Texture::GetSamplerState(SamplerState samplerState)
		{
			ID3D11SamplerState* sampler;
			switch (samplerState)
			{
			case SamplerState::WRAP:
				sampler = s_samplerStates.WRAP;
				break;
			case SamplerState::CLAMP:
				sampler = s_samplerStates.CLAMP;
				break;
			case SamplerState::DECAL:
				sampler = s_samplerStates.DECAL;
				break;
			case SamplerState::MIRROR:
				sampler = s_samplerStates.MIRROR;
				break;
			default:
				sampler = s_samplerStates.WRAP;
				break;
			}
			return sampler;
		}

		Texture * Texture::CreateTexture(SamplerState samplerState, TextureType type, std::string name, ID3D11ShaderResourceView * textureView)
		{
			for (unsigned int i = 0; i < s_loadedTextures.size(); ++i)
			{
				if (s_loadedTextures[i]->GetName() == name && s_loadedTextures[i]->GetTextureType() == type)
					return s_loadedTextures[i];
			}

			Texture* texture = new Texture(samplerState, type, name, textureView);
			if (texture)
				s_loadedTextures.push_back(texture);
			return texture;
		}

		std::string Texture::GetName()
		{
			return m_name;
		}

		ID3D11Resource * Texture::GetTexture()
		{
			return m_data.texture;
		}

		ID3D11ShaderResourceView * Texture::GetTextureView()
		{
			return m_data.textureView;
		}
		Texture::TextureType Texture::GetTextureType()
		{
			return m_textureType;
		}
		void Texture::SetTextureView(ID3D11ShaderResourceView * view)
		{
			m_data.textureView = view;
		}
		bool Texture::Initialized()
		{
			return m_initialized;
		}
		
		Texture::Texture(SamplerState samplerState, int slot, TextureType type, std::string path)
		{
			m_textureType = type;
			m_name = path;
			m_resourceSlot = slot;

			switch (type)
			{
			case TextureType::CUBEMAP:
				m_initialized = utils::D3d::LoadCubeTextureFromFile(ThomasCore::GetDevice(), ThomasCore::GetDeviceContext(), path, m_data.texture, m_data.textureView);
				break;
			default:
				m_initialized = utils::D3d::LoadTextureFromFile(ThomasCore::GetDevice(), ThomasCore::GetDeviceContext(), path, m_data.texture, m_data.textureView);
				break;
			}

			if (m_initialized)
				SetTextureSampler(samplerState);
		}
		Texture::Texture(int mappingMode, TextureType type, std::string path)
		{

			m_textureType = type;
			m_name = path;
			m_initialized = utils::D3d::LoadTextureFromFile(ThomasCore::GetDevice(), ThomasCore::GetDeviceContext(), path, m_data.texture, m_data.textureView);

			if (m_initialized)
				SetTextureSampler(mappingMode);

		}
		Texture::Texture(SamplerState samplerState, TextureType type, std::string path)
		{
			m_textureType = type;
			m_name = path;

			switch (type)
			{
			case TextureType::CUBEMAP:
				m_initialized = utils::D3d::LoadCubeTextureFromFile(ThomasCore::GetDevice(), ThomasCore::GetDeviceContext(), path, m_data.texture, m_data.textureView);
				break;
			default:
				m_initialized = utils::D3d::LoadTextureFromFile(ThomasCore::GetDevice(), ThomasCore::GetDeviceContext(), path, m_data.texture, m_data.textureView);
				break;
			}

			if (m_initialized)
				SetTextureSampler(samplerState);
		}

		Texture::Texture(SamplerState samplerState, int slot, std::string path)
		{
			m_textureType = TextureType::UNDEFINED;
			m_resourceSlot = slot;
			m_name = path;
			m_initialized = utils::D3d::LoadTextureFromFile(ThomasCore::GetDevice(), ThomasCore::GetDeviceContext(), path, m_data.texture, m_data.textureView);
			if (m_initialized)
				SetTextureSampler(samplerState);
		}

		Texture::Texture(SamplerState samplerState, TextureType type, std::string name, ID3D11ShaderResourceView * textureView)
		{
			m_textureType = type;
			m_name = name;
			
			SetTextureSampler(samplerState);
			m_initialized = true;
			m_data.texture = NULL;
			m_data.textureView = textureView;
		}

		Texture::Texture(SamplerState samplerState, int slot, std::string name, ID3D11ShaderResourceView * textureView)
		{
			m_textureType = TextureType::UNDEFINED;
			m_resourceSlot = slot;
			m_name = name;
			SetTextureSampler(samplerState);
			m_initialized = true;
			m_data.texture = NULL;
			m_data.textureView = textureView;
		}

		bool Texture::CreateTextureSamplers()
		{
			D3D11_SAMPLER_DESC samplerDesc;
			samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			samplerDesc.MipLODBias = 0.0f;
			samplerDesc.MaxAnisotropy = 1;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_EQUAL;
			samplerDesc.BorderColor[0] = 1;
			samplerDesc.BorderColor[1] = 1;
			samplerDesc.BorderColor[2] = 1;
			samplerDesc.BorderColor[3] = 1;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;


			HRESULT result;

			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			result = ThomasCore::GetDevice()->CreateSamplerState(&samplerDesc, &s_samplerStates.WRAP);


			if (SUCCEEDED(result))
			{
				samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
				samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
				samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
				result = ThomasCore::GetDevice()->CreateSamplerState(&samplerDesc, &s_samplerStates.CLAMP);
			}

			if (SUCCEEDED(result))
			{
				samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
				samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
				samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
				result = ThomasCore::GetDevice()->CreateSamplerState(&samplerDesc, &s_samplerStates.DECAL);
			}

			if (SUCCEEDED(result))
			{
				samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
				samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
				samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
				result = ThomasCore::GetDevice()->CreateSamplerState(&samplerDesc, &s_samplerStates.MIRROR);
			}

			return SUCCEEDED(result);
		}
		void Texture::SetTextureSampler(int textureMode)
		{
			switch (textureMode)
			{
			case aiTextureMapMode_Wrap:
				m_samplerState = s_samplerStates.WRAP;
				break;
			case aiTextureMapMode_Clamp:
				m_samplerState = s_samplerStates.CLAMP;
				break;
			case aiTextureMapMode_Decal:
				m_samplerState = s_samplerStates.DECAL;
				break;
			case aiTextureMapMode_Mirror:
				m_samplerState = s_samplerStates.MIRROR;
				break;
			default:
				m_samplerState = s_samplerStates.WRAP;
				break;
			}
		}
		void Texture::SetTextureSampler(SamplerState samplerState)
		{
			switch (samplerState)
			{
			case SamplerState::WRAP:
				m_samplerState = s_samplerStates.WRAP;
				break;
			case SamplerState::CLAMP:
				m_samplerState = s_samplerStates.CLAMP;
				break;
			case SamplerState::DECAL:
				m_samplerState = s_samplerStates.DECAL;
				break;
			case SamplerState::MIRROR:
				m_samplerState = s_samplerStates.MIRROR;
				break;
			default:
				m_samplerState = s_samplerStates.WRAP;
				break;
			}
		}
		void Texture::Destroy()
		{
			for (unsigned int i = 0; i < s_loadedTextures.size(); ++i)
			{
				if (s_loadedTextures[i]->m_data.texture)
					s_loadedTextures[i]->m_data.texture->Release();
				if(s_loadedTextures[i]->m_data.textureView)
					s_loadedTextures[i]->m_data.textureView->Release();


				// Clean up heap
				delete s_loadedTextures[i];

			}
			s_loadedTextures.clear();
		
		}

	}
}