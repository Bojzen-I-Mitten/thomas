#include "ComputeShader.h"

namespace thomas
{
	namespace graphics
	{
		ComputeShader::ComputeShader(resource::Shader* shader) : Material(shader)
		{
			
		}
		void ComputeShader::Dispatch(int threadGroupX, int threadGroupY, int threadGroupZ)
		{
			Bind();
			ThomasCore::GetDeviceContext()->Dispatch(threadGroupX, threadGroupY, threadGroupZ);
		}
		void ComputeShader::SetUAV(const std::string & name, ID3D11UnorderedAccessView& value)
		{
			if (HasProperty(name))
			{
				GetProperty(name)->SetUAV(value);
			}
			else
			{
				LOG("Property " << name << " does not exist for material:" << m_name);
			}
		}
		ID3D11UnorderedAccessView * ComputeShader::GetUAV(const std::string & name)
		{
			if (HasProperty(name))
			{
				return GetProperty(name)->GetUAV();
			}
			else
			{
				LOG("Property " << name << " does not exist for material:" << m_name);
				return nullptr;
			}
		}
	}
}