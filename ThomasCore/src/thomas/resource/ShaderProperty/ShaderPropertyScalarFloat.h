#pragma once
#include "ShaderProperty.h"
namespace thomas
{
	namespace resource
	{
		class Shader;
		namespace shaderProperty
		{
			class ShaderPropertyScalarFloat : public ShaderProperty
			{
			public:

				ShaderPropertyScalarFloat(float value) : ShaderProperty(Type::SCALAR_FLOAT), m_value(value) {};
				void Apply(std::string name, Shader* shader)
				{
			
					shader->GetEffect()->GetVariableByName(name.c_str())->AsScalar()->SetFloat(m_value);
				}
				static ShaderProperty* GetDefault() { return new ShaderPropertyScalarFloat(0); }
				float GetValue() { return m_value; }
			private:
				float m_value;
			};
		}
	}
}