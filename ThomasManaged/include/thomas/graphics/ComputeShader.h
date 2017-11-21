#pragma once
#include "Material.h"

namespace thomas
{
	namespace graphics
	{
		class THOMAS_API ComputeShader : public Material
		{
		public:
			ComputeShader(Shader* shader);
			void Dispatch(int threadGroupX, int threadGroupY, int threadGroupZ);
			void SetUAV(const std::string& name, ID3D11UnorderedAccessView& value);
			ID3D11UnorderedAccessView* GetUAV(const std::string& name);
		};
	}
}