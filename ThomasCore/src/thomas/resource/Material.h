#pragma once
#include "../Common.h"
#include "../utils/Math.h"
#include "Resource.h"
namespace thomas
{
	namespace graphics
	{
		class Mesh;
		
	}
	namespace resource
	{
		class Texture;
		class ShaderProperty;
		class Shader;
		class THOMAS_API Material : public Resource
		{
		protected:
			void SetSampler(const std::string name, resource::Texture& value);
			
		private:
			void CreateProperties();
			void OnChanged();
		public:
			static void Init();
			static void Destroy();
			static Material* GetStandardMaterial();

			Material(resource::Shader* shader);
			Material(Material* original);
			Material(std::string path);
			~Material();

			void Bind();
			ShaderProperty* GetProperty(const std::string& name);
			void SetShader(resource::Shader* shader);
			resource::Shader* GetShader();

			bool HasProperty(const std::string& name);

			math::Color* GetColor(const std::string& name);
			void SetColor(const std::string& name, const math::Color& value);

			float* GetFloat(const std::string& name);
			void SetFloat(const std::string& name, float& value);

			int* GetInt(const std::string& name);
			void SetInt(const std::string& name, int& value);

			math::Matrix* GetMatrix(const std::string& name);
			void SetMatrix(const std::string& name, math::Matrix& value);

			resource::Texture* GetTexture(const std::string& name);
			void SetTexture(const std::string& name, resource::Texture& value);

			math::Vector4* GetVector(const std::string& name);
			void SetVector(const std::string& name, math::Vector4& value);

			void SetResource(const std::string& name, ID3D11ShaderResourceView& value);
			void SetBuffer(const std::string& name, ID3D11Buffer& value);

			void SetRaw(const std::string& name, void* value, size_t size, UINT count);
			void SetRaw(const std::string& name, void* value);
			void SetShaderPassEnabled(int index, bool enabled);
			void SetShaderPassEnabled(std::string name, bool enabled);

			//Only enable the specified shader pass
			void SetShaderPass(int index);
			void SetShaderPass(std::string name);

			void Draw(graphics::Mesh* mesh);
			void Draw(UINT vertexCount, UINT startVertexLocation);

			std::vector<ShaderProperty*> GetEditorProperties();
			std::vector<ShaderProperty*> GetAllProperties();

			UINT GetId();

		public:
			int m_renderQueue;
			D3D11_PRIMITIVE_TOPOLOGY m_topology;
		private:
			struct Pass
			{
				std::string name;
				bool enabled;
				int index;
			};
			bool m_isInstance;
			Material* m_baseMaterial;
			UINT m_id;
			resource::Shader* m_shader;
			std::vector<ShaderProperty*> m_properties;
			std::vector<Pass> m_passes;
			static Material* s_standardMaterial;

			static UINT s_idCounter;
		};
	}
}