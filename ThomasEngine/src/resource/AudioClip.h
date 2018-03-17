#pragma once
#pragma once
#pragma unmanaged
#include "thomas\resource\Resource.h"
#include "thomas\resource\AudioClip.h"
#pragma managed
#include <string>
#include <msclr\marshal_cppstd.h>
#include "Resource.h"

namespace ThomasEditor
{
	public ref class AudioClip : public Resource
	{
	public:
		AudioClip(String^ name) : Resource(new thomas::resource::AudioClip(msclr::interop::marshal_as<std::string>(name))) {};
	};
}
