#pragma once
#include "ShaderProperty.h"
namespace thomas
{
	namespace resource
	{
		class Shader;
		namespace shaderProperty
		{
			class ShaderPropertyScalarInt : public ShaderProperty
			{
			public:

				ShaderPropertyScalarInt(int value) : ShaderProperty(Type::SCALAR_INT), m_value(value) {};
				void Apply(std::string name, Shader* shader)
				{
					shader->GetEffect()->GetVariableByName(name.c_str())->AsScalar()->SetInt(m_value);
				}
				static ShaderProperty* GetDefault() { return new ShaderPropertyScalarInt(0); }
				int GetValue() { return m_value; }
			private:
				int m_value;
			};
		}
	}
}