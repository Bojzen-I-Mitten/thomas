#pragma once
#pragma unmanaged
#include <thomas\graphics\Shader.h>
#pragma managed

#include "math/Math.h"
#include "Utility.h"
using namespace System;
using namespace System::IO;
namespace ThomasEditor
{
	public ref class Shader
	{
	internal:
		thomas::graphics::Shader* nativePtr;
		Shader(thomas::graphics::Shader* ptr) { nativePtr = ptr; }
	public:
		
		static Shader^ Find(String^ name) { 
			thomas::graphics::Shader* s = thomas::graphics::Shader::Find(Utility::ConvertString(name));
			if (s)
				return %Shader(s);
			else
				return nullptr;
		}

		static void SetGlobalColor(String^ name, Color value) { thomas::graphics::Shader::SetGlobalColor(Utility::ConvertString(name), thomas::math::Color(value.r, value.g, value.b, value.a)); };
		static void SetGlobalFloat(String^ name, float value) { thomas::graphics::Shader::SetGlobalFloat(Utility::ConvertString(name), value); };;
		static void SetGlobalInt(String^ name, int value) { thomas::graphics::Shader::SetGlobalInt(Utility::ConvertString(name), value); };;
		static void SetGlobalMatrix(String^ name, Matrix4x4 value) { thomas::graphics::Shader::SetGlobalMatrix(Utility::ConvertString(name), value.ToThomas()); };;
		static void SetGlobalVector(String^ name, Vector4 value) { thomas::graphics::Shader::SetGlobalVector(Utility::ConvertString(name), thomas::math::Vector4(value.x, value.y, value.z, value.w)); };;

		//static void SetGlobalTexture(String^ name, Texture& value);

		static void RecompileShaders() { thomas::graphics::Shader::RecompileShaders(); }

		property String^ Name
		{
			String^ get() { return Utility::ConvertString(nativePtr->GetName()); }
		}
	};
}