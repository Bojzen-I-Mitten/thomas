#pragma once

#include "../Common.h"
#include <d3dx11effect.h>
#include <vector>
#include <map>
namespace thomas
{
	namespace graphics
	{
		class THOMAS_API Shader
		{
		public:
			struct ShaderVariable
			{
				std::string name;
				unsigned int index;
				D3DX11_EFFECT_TYPE_DESC desc;
			};
		private:
			static bool Compile(std::string filePath, ID3DX11Effect** effect);

			Shader(std::string name, ID3DX11Effect* effect);
			~Shader();
			void SetupReflection();
			DXGI_FORMAT GetDXGIFormat(BYTE mask, D3D_REGISTER_COMPONENT_TYPE componentType);
			
			
		public:
			static Shader* CreateShader(std::string name, std::string filePath);
			void BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY type);
			void BindVertexBuffer(ID3D11Buffer * vertexBuffer, UINT stride, UINT offset = 0);
			void BindIndexBuffer(ID3D11Buffer * indexBuffer);
			void Bind();
			std::string GetName();

			static void DestroyAllShaders();

			ID3DX11Effect* GetEffect();
			std::vector<ShaderVariable>* GetVariables();

			int PropertyToID(const std::string& name);

		
		private:

			std::string m_name;
			ID3DX11Effect* m_effect;
			std::vector<ShaderVariable> m_variables;
			ID3D11InputLayout* m_inputLayout;
			
			static std::vector<Shader*> s_loadedShaders;

		};
	}
}
