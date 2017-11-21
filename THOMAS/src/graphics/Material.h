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
			void SetProperties();
			void SetSampler(const std::string name, Texture& value);
			MaterialProperty* GetProperty(const std::string& name);
			void Bind();
		public:
			Material(Shader* shader);
			void SetShader(Shader* shader);
			Shader* GetShader();
			std::string GetName();
			void SetName(std::string name);

			bool HasProperty(const std::string& name);

			math::Color* GetColor(const std::string& name);
			void SetColor(const std::string& name, math::Color& value);

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

			void Draw(Mesh* mesh);
			void Draw(UINT vertexCount, UINT startVertexLocation);


			static Material* Find(std::string name);

		public:
			int m_renderQueue;
			std::string m_name;
			D3D11_PRIMITIVE_TOPOLOGY m_topology;
		private:
			Shader* m_shader;
			std::vector<MaterialProperty*> m_properties;
		};
	}
}