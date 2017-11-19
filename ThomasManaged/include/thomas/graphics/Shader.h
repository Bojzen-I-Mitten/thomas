#pragma once
#include "../Common.h"
#include <d3d11.h>
#include <string>
#include <vector>
namespace thomas
{
	class Scene;
	namespace graphics
	{

		class THOMAS_API Shader
		{
		public:
			enum class InputLayouts
			{
				STANDARD = 0,
				POST_EFFECT = 1,
				NONE = 2
			};
			enum class ResourceType
			{
				GAME_OBJECT = 0,
				MATERIAL = 1,
				LIGHTS = 2
			};
		private:

			bool CreateInputLayout(InputLayouts layout);
			Shader(std::string name, InputLayouts inputLayout, std::string filePath, Scene* scene);
			Shader(std::string name, InputLayouts inputLayout, std::string vertexShader, std::string geometryShader, 
				std::string hullShader, std::string domainShader, std::string pixelShader, Scene* scene);
			Shader(std::string name, std::string ComputeShader, Scene* scene);
			void ReloadShader();
		public:



			~Shader();
			bool Bind();
			bool Unbind();
			std::string GetName();

			bool BindBuffer(ID3D11Buffer* resource, ResourceType type);
			bool BindBuffer(ID3D11Buffer* resource, int slot);
			bool BindUAV(ID3D11UnorderedAccessView* uav, int slot);
			bool BindResource(ID3D11ShaderResourceView* texture, int slot);
			bool BindTextureSampler(ID3D11SamplerState* sampler, int slot);
			bool BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY type);
			bool BindVertexBuffer(ID3D11Buffer* vertexBuffer, UINT stride, UINT offset);
			bool BindIndexBuffer(ID3D11Buffer* indexBuffer);

			static ID3DBlob* Compile(std::string source, std::string profile, std::string main);

			static Shader* CreateShader(std::string name, InputLayouts inputLayout, std::string filePath, 
				Scene* scene);
			static Shader* CreateShader(std::string name, InputLayouts inputLayout,
				std::string vertexShader, 
				std::string geometryShader,
				std::string hullShader,
				std::string domainShader, 
				std::string pixelShader,
				Scene* scene);

			static Shader* CreateComputeShader(std::string name, std::string computeShader, Scene* scene);

			static Shader* GetCurrentBoundShader();
			static Shader* GetShaderByName(std::string name);
			static std::vector<Shader*> GetShadersByScene(Scene* scene);
			static std::vector<Shader*> GetLoadedShaders();
			static void ReloadShaders();
			static bool Destroy();
			static void Destroy(Scene* scene);

		private:
			struct Data
			{
				ID3DBlob* vs;
				ID3D11VertexShader* vertexShader;
				std::string VSfilePath;
				ID3DBlob* ps;
				ID3D11PixelShader* pixelShader;
				std::string PSfilePath;
				ID3DBlob* gs;
				ID3D11GeometryShader* geometryShader;
				std::string GSfilePath;
				ID3DBlob* hs;
				ID3D11HullShader* hullShader;
				std::string HSfilePath;
				ID3DBlob* ds;
				ID3D11DomainShader* domainShader;
				std::string DSfilePath;
				ID3D11InputLayout* inputLayout;
				ID3DBlob* cs;
				std::string CSFilePath;
				ID3D11ComputeShader* computeShader;
			};

			Data m_data;
			std::string m_name;
			Scene* m_scene;

			static Shader* s_currentBoundShader;
			static std::vector<Shader*> s_loadedShaders;
		};
	}
}