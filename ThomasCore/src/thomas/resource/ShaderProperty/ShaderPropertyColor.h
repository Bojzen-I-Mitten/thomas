#pragma once
#include "ShaderProperty.h"
#include "../../utils/Math.h"
namespace thomas
{
	namespace resource
	{
		namespace shaderProperty
		{
			class ShaderPropertyColor : public ShaderProperty
			{
			public:

				ShaderPropertyColor(math::Color value) : ShaderProperty(Type::COLOR), m_value(value) {};
				void Apply(std::string name, Shader* shader)
				{
					HRESULT result = shader->GetEffect()->GetVariableByName(name.c_str())->AsVector()->SetFloatVector((float*)&m_value);
				}
				static ShaderProperty* GetDefault() { return new ShaderPropertyColor(math::Color(1, 1, 1, 1)); }
				math::Color GetValue() { return m_value; }
			private:
				math::Color m_value;
			};
		}
	}
}