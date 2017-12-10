#pragma once

#include "../Common.h"
#include <d3dx11effect.h>
#include <vector>
#include <map>
#include "../utils/Math.h"
namespace thomas
{
	namespace graphics
	{
		class MaterialProperty;
		class Texture;
		class THOMAS_API Shader
		{
		public:

		private:
			static bool Compile(std::string filePath, ID3DX11Effect** effect);

			Shader(std::string name, ID3DX11Effect* effect, std::string filePath);
			~Shader();
			void SetupReflection();
			DXGI_FORMAT GetDXGIFormat(BYTE mask, D3D_REGISTER_COMPONENT_TYPE componentType);
			
			void Destroy();
			
		public:
			struct ShaderPass
			{
				std::string name;
				ID3D11InputLayout* inputLayout;
			};

			static bool Init();
			static Shader* GetStandardShader();

			static Shader* CreateShader(std::string name, std::string filePath);
			void BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY type);
			void BindVertexBuffer(ID3D11Buffer * vertexBuffer, UINT stride, UINT offset = 0);
			void BindIndexBuffer(ID3D11Buffer * indexBuffer);
			void Bind();
			std::string GetName();

			std::vector<ShaderPass>* GetPasses();
			void SetPass(int passIndex);

			static void DestroyAllShaders();

			static void SetGlobalColor(const std::string& name, math::Color value);

			static void SetGlobalFloat(const std::string& name, float value);

			static void SetGlobalInt(const std::string& name, int value);

			static void SetGlobalMatrix(const std::string& name, math::Matrix value);

			static void SetGlobalTexture(const std::string& name, Texture& value);

			static void SetGlobalVector(const std::string& name, math::Vector4 value);

			static Shader* Find(const std::string& name);

			ID3DX11Effect* GetEffect();
			bool HasProperty(const std::string& name);
			MaterialProperty* GetProperty(const std::string& name);

			void Recompile();
			static void RecompileShaders();

		private:
			std::string m_filePath;
			std::string m_name;
			ID3DX11Effect* m_effect;
			std::vector<MaterialProperty*> m_properties;
			std::vector<ShaderPass> m_passes;
			
			static std::vector<Shader*> s_loadedShaders;
			static Shader* s_standardShader;

		};
	}
}
