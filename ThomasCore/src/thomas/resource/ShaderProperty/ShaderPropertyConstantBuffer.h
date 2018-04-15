#pragma once
#include "ShaderProperty.h"
#include "../../utils/d3d.h"
namespace thomas
{
	namespace resource
	{
		class Shader;
		namespace shaderProperty
		{
			class ShaderPropertyConstantBuffer : public ShaderProperty
			{
			public:

				ShaderPropertyConstantBuffer(ID3D11Buffer* value) : ShaderProperty(Type::CONSTANT_BUFFER), m_value(value) {};
				void Apply(std::string name, Shader* shader)
				{
					shader->GetEffect()->GetVariableByName(name.c_str())->AsConstantBuffer()->SetConstantBuffer(m_value);
				}
				static ShaderProperty* GetDefault() { return new ShaderPropertyConstantBuffer(nullptr); }
				ID3D11Buffer* GetValue() { return m_value; }
			private:
				ID3D11Buffer * m_value;
			};
		}
	}
}