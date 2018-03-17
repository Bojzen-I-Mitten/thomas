#pragma once

#include <string>
#include <vector>
#include "../Common.h"
#include <d3d11.h>
namespace thomas
{
	namespace graphics
	{
		class THOMAS_API Texture
		{

		public:
			//represents texture slot
			enum class TextureType
			{
				DIFFUSE = 0,
				SPECULAR = 1,
				NORMAL = 2,
				HEIGHT_MAP = 3,
				CUBEMAP = 4,
				UNDEFINED = 100
			};

			enum class SamplerState {
				WRAP = 0,
				CLAMP = 1,
				DECAL = 2,
				MIRROR = 3
			};


		private:
			Texture(int mappingMode, TextureType type, std::string path);
			Texture(SamplerState samplerState, TextureType type, std::string path);
			Texture(SamplerState samplerState, TextureType type, std::string name, ID3D11ShaderResourceView* textureView);
			Texture(SamplerState samplerState, int slot, std::string name, ID3D11ShaderResourceView* textureView);
			Texture(SamplerState samplerState, int slot, std::string path);
			static bool CreateTextureSamplers();
			void SetTextureSampler(int textureMode);
			void SetTextureSampler(SamplerState samplerState);
		public:
			static void ReleaseSamplers(); // Called upon exiting thomas core
			static bool Init();
			static Texture* CreateTexture(int mappingMode, TextureType type, std::string path);
			static Texture* CreateTexture(SamplerState samplerState, TextureType type, std::string path);
			static Texture* CreateTexture(SamplerState samplerState, TextureType type, std::string name, ID3D11ShaderResourceView* textureView);
			static Texture* CreateTexture(SamplerState samplerState, int slot, std::string name, ID3D11ShaderResourceView* textureView);
			static Texture* CreateTexture(SamplerState samplerState, int slot, std::string path);
			static Texture * CreateTexture(SamplerState samplerState, int slot, TextureType type, std::string path);

			static ID3D11SamplerState* GetSamplerState(SamplerState samplerState);
			ID3D11SamplerState* GetSamplerState();
			std::string GetName();
			ID3D11Resource* GetTexture();
			ID3D11ShaderResourceView* GetTextureView();
			TextureType GetTextureType();

			void SetTextureView(ID3D11ShaderResourceView* view);
			bool Initialized();


			Texture(SamplerState samplerState, int slot, TextureType type, std::string path);

			static void Destroy();
		private:
			struct TextureData
			{
				ID3D11ShaderResourceView* textureView;
				ID3D11Resource* texture;
			};

			struct SamplerStates {
				ID3D11SamplerState* WRAP;
				ID3D11SamplerState* CLAMP;
				ID3D11SamplerState* DECAL;
				ID3D11SamplerState* MIRROR;
			};
			
			int m_resourceSlot;
			std::string m_name;
			TextureData m_data;
			bool m_initialized;
			static std::vector<Texture*> s_loadedTextures;
			static SamplerStates s_samplerStates;
			ID3D11SamplerState* m_samplerState;
			TextureType m_textureType;
		};
	}
}