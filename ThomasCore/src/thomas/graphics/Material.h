#pragma once
#include "../Common.h"
#include "../utils/Math.h"
#include "MaterialProperty.h"
namespace thomas
{
	namespace graphics
	{
		class Shader;
		class Texture;
		class Mesh;

		class THOMAS_API Material
		{
		protected:
			void SetSampler(const std::string name, Texture& value);
			
			
		public:
			static void Init();
			static void Destroy();
			static Material* GetStandardMaterial();

			Material(Shader* shader);
			Material(std::string name, Shader* shader);
			Material(Material* original);
			~Material();

			void Bind();
			MaterialProperty* GetProperty(const std::string& name);
			void SetShader(Shader* shader);
			Shader* GetShader();
			std::string GetName();
			void SetName(std::string name);

			bool HasProperty(const std::string& name);

			math::Color* GetColor(const std::string& name);
			void SetColor(const std::string& name, const math::Color& value);

			float* GetFloat(const std::string& name);
			void SetFloat(const std::string& name, float& value);

			int* GetInt(const std::string& name);
			void SetInt(const std::string& name, int& value);

			math::Matrix* GetMatrix(const std::string& name);
			void SetMatrix(const std::string& name, math::Matrix& value);

			Texture* GetTexture(const std::string& name);
			void SetTexture(const std::string& name, Texture& value);

			math::Vector4* GetVector(const std::string& name);
			void SetVector(const std::string& name, math::Vector4& value);

			void SetResource(const std::string& name, ID3D11ShaderResourceView& value);
			void SetBuffer(const std::string& name, ID3D11Buffer& value);

			void SetRaw(const std::string& name, void* value, size_t size, UINT count);

			void SetShaderPassEnabled(int index, bool enabled);
			void SetShaderPassEnabled(std::string name, bool enabled);

			//Only enable the specified shader pass
			void SetShaderPass(int index);
			void SetShaderPass(std::string name);

			void Draw(Mesh* mesh);
			void Draw(UINT vertexCount, UINT startVertexLocation);

			std::vector<MaterialProperty*> GetEditorProperties();

			Material* GetBaseMaterial();
			static Material* Find(std::string name);

			static std::vector<Material*>* GetMaterialInstances();

			UINT GetId();

		public:
			int m_renderQueue;
			std::string m_name;
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
			Shader* m_shader;
			std::vector<MaterialProperty*> m_properties;
			std::vector<Pass> m_passes;

			static std::vector<Material*> s_materials;
			static std::vector<Material*> s_materialInstances;

			static Material* s_standardMaterial;

			static UINT s_idCounter;
		};
	}
}