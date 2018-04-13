#pragma once

#include "../Common.h"
#include <d3dx11effect.h>
#include <vector>
#include <map>
#include "../utils/Math.h"
#include "Resource.h"
namespace thomas
{
	namespace graphics
	{
		class Texture;
	}
	namespace resource
	{
		class ShaderProperty;
		class THOMAS_API Shader : public Resource
		{
		public:

		private:
			static bool Compile(std::string path, ID3DX11Effect** effect);

			Shader(ID3DX11Effect* effect, std::string path);
			~Shader();
			void SetupReflection();
			DXGI_FORMAT GetDXGIFormat(BYTE mask, D3D_REGISTER_COMPONENT_TYPE componentType);
			
			void Destroy();
			static void RecompileShaders();

			void OnChanged();
		public:
			struct ShaderPass
			{
				std::string name;
				ID3D11InputLayout* inputLayout;
			};

			static bool Init();
			static Shader* GetStandardShader();

			static Shader* CreateShader(std::string path);
			void BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY type);
			void BindVertexBuffer(ID3D11Buffer * vertexBuffer, UINT stride, UINT offset = 0);
			void BindIndexBuffer(ID3D11Buffer * indexBuffer);
			void Bind();
			std::vector<ShaderPass>* GetPasses();
			void SetPass(int passIndex);

			static void DestroyAllShaders();

			static void SetGlobalColor(const std::string& name, math::Color value);

			static void SetGlobalFloat(const std::string& name, float value);

			static void SetGlobalInt(const std::string& name, int value);

			static void SetGlobalMatrix(const std::string& name, math::Matrix value);

			static void SetGlobalTexture(const std::string& name, graphics::Texture& value);

			static void SetGlobalVector(const std::string& name, math::Vector4 value);

			static Shader* FindByName(const std::string& name);
			static Shader* FindByPath(const std::string& path);

			ID3DX11Effect* GetEffect();
			bool HasProperty(const std::string& name);
			ShaderProperty* GetProperty(const std::string& name);
			std::vector<ShaderProperty*> GetProperties();
			static void Update();
			void Recompile();
			static void QueueRecompile();
		private:
			ID3DX11Effect* m_effect;
			std::vector<ShaderProperty*> m_properties;
			std::vector<ShaderPass> m_passes;
			
			static std::vector<Shader*> s_loadedShaders;
			static Shader* s_standardShader;
			static bool s_shouldRecompile;


			class ShaderInclude : public ID3DInclude
			{
				public:
					ShaderInclude(const char* shaderDir, const char* systemDir);
					// Inherited via ID3DInclude
					virtual HRESULT __stdcall Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID * ppData, UINT * pBytes) override;
					virtual HRESULT __stdcall Close(LPCVOID pData) override;

			private:
				std::string m_shaderDir;
				std::string m_systemDir;
			};
		};
	}
}
