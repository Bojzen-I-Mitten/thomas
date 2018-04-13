#pragma once
#include "ShaderProperty.h"
namespace thomas
{
	namespace resource
	{
		class Shader;
		namespace shaderProperty
		{
			class ShaderPropertyScalarBool : public ShaderProperty
			{
			public:

				ShaderPropertyScalarBool(bool value) : ShaderProperty(Type::SCALAR_BOOL), m_value(value) {};
				void Apply(std::string name, Shader* shader)
				{
					shader->GetEffect()->GetVariableByName(name.c_str())->AsScalar()->SetBool(m_value);
				}
				static ShaderProperty* GetDefault() { return new ShaderPropertyScalarBool(0); }
				bool GetValue() { return m_value; }
			private:
				bool m_value;
			};
		}
	}
}