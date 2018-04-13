#pragma once
#pragma unmanaged
#include "thomas\resource\AudioClip.h"
#pragma managed

#include "Resource.h"

namespace ThomasEditor
{
	[DataContractAttribute]
	public ref class AudioClip : public Resource
	{
	public:
		AudioClip(String^ path) : Resource(path, new thomas::resource::AudioClip(Utility::ConvertString(path))) {};

		[OnDeserializedAttribute]
		void OnDeserialized(StreamingContext c)
		{
			m_nativePtr = new thomas::resource::AudioClip(Utility::ConvertString(m_path));
		}
	};
}
