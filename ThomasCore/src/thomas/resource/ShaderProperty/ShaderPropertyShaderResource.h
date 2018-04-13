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
			class ShaderPropertyShaderResource : public ShaderProperty
			{
			public:

				ShaderPropertyShaderResource(ID3D11ShaderResourceView* value) : ShaderProperty(Type::SHADER_RESOURCE), m_value(value) {};
				void Apply(std::string name, Shader* shader)
				{
					shader->GetEffect()->GetVariableByName(name.c_str())->AsShaderResource()->SetResource(m_value);
				}
				static ShaderProperty* GetDefault() { return new ShaderPropertyShaderResource(nullptr); }
				ID3D11ShaderResourceView* GetValue() { return m_value; }
			private:
				ID3D11ShaderResourceView* m_value;
			};
		}
	}
}