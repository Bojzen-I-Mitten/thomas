#pragma once
#pragma unmanaged
#include "thomas\resource\Resource.h"
#pragma managed
#include <msclr\marshal_cppstd.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace System::ComponentModel;

namespace ThomasEditor
{
	public ref class Resource
	{
	internal:
		thomas::resource::Resource* native_ptr;
	public:
		Resource(thomas::resource::Resource* ptr)
		{
			native_ptr = ptr;
		}
	};
}