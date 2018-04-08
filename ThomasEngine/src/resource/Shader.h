#pragma once
#pragma unmanaged
#include <thomas\resource\Shader.h>
#pragma managed
#include "resource.h"
#include "../math/Math.h"
namespace ThomasEditor
{
	[DataContractAttribute]
	public ref class Shader : public Resource
	{
	internal:
		Shader(thomas::resource::Shader* ptr) : Resource(Utility::ConvertString(ptr->GetPath()), ptr) {};
		Shader(String^ path) : Resource(path, thomas::resource::Shader::CreateShader(Utility::ConvertString(path))) {};
	public:
		
		
		static Shader^ Find(String^ name) { 
			thomas::resource::Shader* s = thomas::resource::Shader::FindByName(Utility::ConvertString(name));
			if (s)
				return %Shader(s);
			else
				return nullptr;
		}

		static void SetGlobalColor(String^ name, Color value) { thomas::resource::Shader::SetGlobalColor(Utility::ConvertString(name), thomas::math::Color(value.r, value.g, value.b, value.a)); };
		static void SetGlobalFloat(String^ name, float value) { thomas::resource::Shader::SetGlobalFloat(Utility::ConvertString(name), value); };;
		static void SetGlobalInt(String^ name, int value) { thomas::resource::Shader::SetGlobalInt(Utility::ConvertString(name), value); };;
		static void SetGlobalMatrix(String^ name, Matrix4x4 value) { thomas::resource::Shader::SetGlobalMatrix(Utility::ConvertString(name), value.ToThomas()); };;
		static void SetGlobalVector(String^ name, Vector4 value) { thomas::resource::Shader::SetGlobalVector(Utility::ConvertString(name), thomas::math::Vector4(value.x, value.y, value.z, value.w)); };;

		//static void SetGlobalTexture(String^ name, Texture& value);

		static void RecompileShaders() { thomas::resource::Shader::QueueRecompile(); }

		property String^ Name
		{
			String^ get() { return Utility::ConvertString(m_nativePtr->GetName()); }
		}
		
		[OnDeserializedAttribute]
		void OnDeserialized(StreamingContext c)
		{
			m_nativePtr = thomas::resource::Shader::CreateShader(Utility::ConvertString(m_path));
		}

	};
}