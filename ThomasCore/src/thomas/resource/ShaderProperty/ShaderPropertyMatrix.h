#pragma once
#include "ShaderProperty.h"
#include "../../utils/Math.h"
namespace thomas
{
	namespace resource
	{
		class Shader;
		namespace shaderProperty
		{
			class ShaderPropertyMatrix : public ShaderProperty
			{
			public:

				ShaderPropertyMatrix(math::Matrix value) : ShaderProperty(Type::MATRIX), m_value(value) {};
				void Apply(std::string name, Shader* shader)
				{
					shader->GetEffect()->GetVariableByName(name.c_str())->AsMatrix()->SetMatrix(*m_value.m);
				}
				static ShaderProperty* GetDefault() { return new ShaderPropertyMatrix(math::Matrix()); }
				math::Matrix GetValue() { return m_value; }
			private:
				math::Matrix m_value;
			};
		}
	}
}