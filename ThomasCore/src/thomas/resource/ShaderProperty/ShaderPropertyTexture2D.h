#pragma once
#include "ShaderProperty.h"
#include "../texture/Texture2D.h"
namespace thomas
{
	namespace resource
	{
		class Shader;
		namespace shaderProperty
		{
			class ShaderPropertyTexture2D : public ShaderProperty
			{
			public:

				ShaderPropertyTexture2D(Texture2D* value) : ShaderProperty(Type::TEXTURE2D), m_value(value) {};
				void Apply(std::string name, Shader* shader)
				{
					if (m_value)
					{
						shader->GetEffect()->GetVariableByName(name.c_str())->AsShaderResource()->SetResource(m_value->GetResourceView());
					}
				}
				static ShaderProperty* GetDefault() { return new ShaderPropertyTexture2D(nullptr); }
				Texture2D* GetValue() { return m_value; }
			private:
				Texture2D* m_value;
			};
		}
	}
}