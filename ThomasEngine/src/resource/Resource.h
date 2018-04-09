#pragma once
#pragma unmanaged
#include "thomas\resource\Resource.h"
#include <memory>
#pragma managed

#include "../Utility.h"

using namespace System;
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

		Resource(String^ path, thomas::resource::Resource* ptr)
		{
			m_path = path;
			m_nativePtr = ptr;
		}
		virtual ~Resource()
		{
		}
	public:
		
		

		String ^ GetPath()
		{
			return m_path;
		}

		String^ ToString() override
		{
			return System::IO::Path::GetFileNameWithoutExtension(m_path);
		}
	};
}