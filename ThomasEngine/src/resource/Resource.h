#pragma once
#pragma unmanaged
#include "thomas\resource\Resource.h"
#pragma managed
#include <msclr\marshal_cppstd.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace System::ComponentModel;
using namespace System::Runtime::Serialization;

namespace ThomasEditor
{
	[DataContractAttribute]
	public ref class Resource
	{
	internal:
		thomas::resource::Resource* m_nativePtr;

		[DataMemberAttribute]
		String^ m_path;
	public:
		Resource(String^ path, thomas::resource::Resource* ptr)
		{
			m_path = path;
			m_nativePtr = ptr;
		}

		String^ ToString() override
		{
			return System::IO::Path::GetFileNameWithoutExtension(m_path);
		}
	};
}