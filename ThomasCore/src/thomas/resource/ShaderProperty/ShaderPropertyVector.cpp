#include "ShaderPropertyVector.h"
#include "../Shader.h"
void thomas::resource::shaderProperty::ShaderPropertyVector::Apply(std::string name, Shader * shader)
{
	shader->GetEffect()->GetVariableByName(name.c_str())->AsVector()->SetFloatVector((float*)&m_value);
}
