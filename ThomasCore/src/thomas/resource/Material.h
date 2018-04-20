#pragma once
#include "../Common.h"
#include "../utils/Math.h"
#include "Resource.h"
#include <memory>
#include <map>
namespace thomas
{
	namespace graphics
	{
		class Mesh;
		
	}
	namespace resource
	{
		namespace shaderProperty { class ShaderProperty; }
		class Texture2D;
		class Shader;
		class THOMAS_API Material : public Resource
		{
		protected:
			
		private:
			void FetchPropertiesFromShader();
			void OnChanged();
			void Lock();
			void Unlock();
		public:
			static void Init();
			static void Destroy();
			static Material* GetStandardMaterial();

			Material(resource::Shader* shader);
			Material(Material* original);
			Material(std::string path);
			~Material();

			void Bind();
			std::shared_ptr<shaderProperty::ShaderProperty> GetProperty(const std::string& name);
			void SetShader(resource::Shader* shader);
			resource::Shader* GetShader();


			void ApplyProperty(const std::string& name);
			bool HasProperty(const std::string& name);
			math::Color GetColor(const std::string& name);
			void SetColor(const std::string& name, const math::Color& value);

			float GetFloat(const std::string& name);
			void SetFloat(const std::string& name, float& value);

			int GetInt(const std::string& name);
			void SetInt(const std::string& name, int& value);

			math::Matrix GetMatrix(const std::string& name);
			void SetMatrix(const std::string& name, math::Matrix& value);

			resource::Texture2D* GetTexture2D(const std::string& name);
			void SetTexture2D(const std::string& name, resource::Texture2D* value);

			math::Vector4 GetVector(const std::string& name);
			void SetVector(const std::string& name, math::Vector4& value);

			void SetResource(const std::string& name, ID3D11ShaderResourceView* value);
			void SetConstantBuffer(const std::string& name, ID3D11Buffer* value);

			void SetShaderPassEnabled(int index, bool enabled);
			void SetShaderPassEnabled(std::string name, bool enabled);

			//Only enable the specified shader pass
			void SetShaderPass(int index);
			void SetShaderPass(std::string name);

			void Draw(std::shared_ptr<graphics::Mesh> mesh);
			void Draw(UINT vertexCount, UINT startVertexLocation);

			std::map<std::string, std::shared_ptr<shaderProperty::ShaderProperty>> GetEditorProperties();
			std::map<std::string, std::shared_ptr<shaderProperty::ShaderProperty>> GetAllProperties();

			UINT GetId();
			friend bool operator<(const Material& mat1, const Material& mat2);
			bool operator<(const Material& other);
			bool operator<(const Material* other);

		public:
			int m_renderQueue = 2000;
			D3D11_PRIMITIVE_TOPOLOGY m_topology;
		private:
			struct Pass
			{
				std::string name;
				bool enabled;
				int index;
			};
			void* m_lock;
			bool m_isInstance;
			Material* m_baseMaterial;
			UINT m_id;
			resource::Shader* m_shader;
			std::map<std::string, std::shared_ptr<shaderProperty::ShaderProperty>> m_properties;
			std::vector<Pass> m_passes;
			static Material* s_standardMaterial;

			static UINT s_idCounter;
		};
	}
}