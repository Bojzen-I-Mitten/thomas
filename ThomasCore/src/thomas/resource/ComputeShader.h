#pragma once
#include "Material.h"

namespace thomas
{
	namespace resource
	{
		//rework
		class THOMAS_API ComputeShader
		{
		public:
			ComputeShader();
			void Dispatch(int threadGroupX, int threadGroupY, int threadGroupZ);
			void SetUAV(const std::string& name, ID3D11UnorderedAccessView& value);
			ID3D11UnorderedAccessView* GetUAV(const std::string& name);
		};
	}
}