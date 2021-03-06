#pragma once

#include "../Common.h"
#include <d3dx11effect.h>
#include <vector>
#include <map>
#include "../utils/Math.h"
#include "Resource.h"
#include "../utils/Buffers.h"
#include <memory>
namespace thomas
{
	namespace resource
	{
		namespace shaderProperty { class ShaderProperty; }
		class Texture2D;
		class THOMAS_API Shader : public Resource
		{
		public:
			enum class Semantics
			{
				POSITION = 0,
				TEXCOORD = 1,
				NORMAL = 2,
				TANGENT = 3,
				BITANGENT = 4, //Remove?
				BINORMAL = 5,
				BLENDINDICES = 6,
				BLENDWEIGHT = 7,
				COLOR = 8,
				POSITIONT = 9,
				PSIZE = 10,
				UNKNOWN = 11,
				BONEWEIGHT = 12
			};
		private:
			static bool Compile(std::string path, ID3DX11Effect** effect);

			Shader(ID3DX11Effect* effect, std::string path);
			~Shader();
			void SetupReflection();
			DXGI_FORMAT GetDXGIFormat(BYTE mask, D3D_REGISTER_COMPONENT_TYPE componentType);
			
			void Destroy();
			static void RecompileShaders();

			void OnChanged();

			Semantics GetSemanticFromName(std::string semanticName);
			void AddProperty(ID3DX11EffectVariable* prop);
		public:
			struct ShaderPass
			{
				std::string name;
				ID3D11InputLayout* inputLayout;
				std::vector<Semantics> inputSemantics;
			};

			static bool Init();
			static Shader* GetStandardShader();

			static Shader* CreateShader(std::string path);
			void BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY type);
			void BindVertexBuffer(utils::buffers::VertexBuffer* buffer);
			void BindVertexBuffers(std::vector<utils::buffers::VertexBuffer*> buffers);
			void BindIndexBuffer(utils::buffers::IndexBuffer* indexBuffer);
			void Bind();
			std::vector<ShaderPass>* GetPasses();
			void SetPass(int passIndex);
			ShaderPass& GetCurrentPass();
			static void DestroyAllShaders();

			static void SetGlobalColor(const std::string& name, math::Color value);

			static void SetGlobalFloat(const std::string& name, float value);

			static void SetGlobalInt(const std::string& name, int value);

			static void SetGlobalMatrix(const std::string& name, math::Matrix value);
			
			static void SetGlobalTexture2D(const std::string& name, resource::Texture2D* value);
			static void SetGlobalResource(const std::string& name, ID3D11ShaderResourceView* value);
			static void SetGlobalConstantBuffer(const std::string& name, ID3D11Buffer* value);

			static void SetGlobalVector(const std::string& name, math::Vector4 value);

			static Shader* FindByName(const std::string& name);
			static Shader* FindByPath(const std::string& path);


			std::vector<std::string> GetMaterialProperties();
			ID3DX11Effect* GetEffect();
			bool HasProperty(const std::string& name);
			std::shared_ptr<shaderProperty::ShaderProperty> GetProperty(const std::string& name);
			std::map<std::string, std::shared_ptr<shaderProperty::ShaderProperty>> GetProperties();
			static void Update();
			void Recompile();
			static void QueueRecompile();
		private:
			ID3DX11Effect* m_effect;
			std::map<std::string, std::shared_ptr<shaderProperty::ShaderProperty>> m_properties;
			std::vector<ShaderPass> m_passes;
			ShaderPass* m_currentPass;
			std::vector<std::string> m_materialProperties;
			static std::vector<Shader*> s_loadedShaders;
			static Shader* s_standardShader;
			static Shader* s_failedShader;
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
