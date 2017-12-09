#pragma once
#include <string>
#include <msclr\marshal_cppstd.h>
using namespace System;
namespace ThomasEditor
{
	class Utility
	{
	public:
		static std::string ConvertString(String^ string)
		{
			return msclr::interop::marshal_as<std::string>(string);
		}

		static String^ ConvertString(std::string string)
		{
			return gcnew String(string.c_str());
		}
	};

}