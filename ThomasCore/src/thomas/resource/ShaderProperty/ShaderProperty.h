#pragma once
#include <string>
#include "../Shader.h"
namespace thomas
{
	namespace resource
	{
		class Shader;
		namespace shaderProperty
		{
			class ShaderProperty
			{
			public:
				enum class Type
				{
					SCALAR_FLOAT,
					SCALAR_BOOL,
					SCALAR_INT,
					VECTOR,
					MATRIX,
					SHADER_RESOURCE,
					CONSTANT_BUFFER,
					TEXTURE2D
				};

				ShaderProperty(Type type) : m_type(type) {};

				virtual void Apply(std::string name, Shader* shader) = 0;
				void Apply(Shader* shader) { Apply(m_name, shader); }
				Type GetType() { return m_type; }

				bool isMaterialProperty;
				std::string GetName() { return m_name; }
				void SetName(std::string name) {m_name = name;}
			private:
				std::string m_name;
				Type m_type;
			};
		}
	}
}