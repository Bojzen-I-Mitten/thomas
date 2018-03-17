#pragma once
#include "Resource.h"

namespace thomas
{
	namespace resource
	{
		class AudioClip : public Resource
		{
		public:
			AudioClip(std::string name);
			~AudioClip();
		};
	}
}