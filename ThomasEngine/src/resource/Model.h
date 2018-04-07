#pragma once
#pragma unmanaged
#include <thomas\resource\Model.h>
#pragma managed
#include "Resource.h"

namespace ThomasEditor
{
	[DataContractAttribute]
	public ref class Model : public Resource
	{
	public:
		Model(String^ path) : Resource(path, new thomas::resource::Model(Utility::ConvertString(path))) {};

		[OnDeserializedAttribute]
		void OnDeserialized(StreamingContext c)
		{

			m_nativePtr = new thomas::resource::Model(Utility::ConvertString(m_path));
		}
	};
}
