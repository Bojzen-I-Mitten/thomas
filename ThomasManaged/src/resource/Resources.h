#pragma once
#pragma unmanaged
#include "thomas\resource\Resource.h"
#pragma managed
#include <msclr\marshal_cppstd.h>
#include "Resource.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::ComponentModel;

namespace ThomasEditor
{
	public ref class Resources
	{
	internal:
	public:
		generic<typename T>
		where T : Resource
		static T Load(String^ name) {T test = (T)Activator::CreateInstance(T::typeid, name); return test; };
	};
}