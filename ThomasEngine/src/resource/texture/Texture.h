#pragma once
#pragma unmanaged
#include <thomas\resource\texture\Texture.h>
#pragma managed

#include "..\Resource.h"

namespace ThomasEngine
{
	[DataContractAttribute]
	public ref class Texture : public Resource
	{
	public:
		enum class WrapMode {
			Wrap,
			Clamp,
			Decal,
			Mirror
		};

		enum class Dimension {
			Unknown = -1,
			None = 0,
			Tex1D = 1,
			Tex2D = 2,
			Tex3D = 3,
			Cube = 4,
			Any = 6
		};

		enum class FilterMode {
			Point,
			Bilinear,
			Trilinear
		};


	internal:
		Texture(String^ path) : Resource(path, new thomas::resource::Texture(Utility::ConvertString(path))) {};
		Texture(String^ path, thomas::resource::Texture* nativePtr) : Resource(path, nativePtr) {};

	public:
		property int width
		{
			int get() { return ((thomas::resource::Texture*)m_nativePtr)->GetWidth(); }
		}
		property int height
		{
			int get() { return ((thomas::resource::Texture*)m_nativePtr)->GetHeight(); }
		}

		property WrapMode wrapMode 
		{
			WrapMode get(){return (WrapMode)((thomas::resource::Texture*)m_nativePtr)->GetWrapMode();}
			void set(WrapMode value){ ((thomas::resource::Texture*)m_nativePtr)->SetWrapMode((thomas::resource::Texture::WrapMode)value); }
		}

		property FilterMode filterMode
		{
			FilterMode get() { return (FilterMode)((thomas::resource::Texture*)m_nativePtr)->GetFilterMode(); }
			void set(FilterMode value) { ((thomas::resource::Texture*)m_nativePtr)->SetFilterMode((thomas::resource::Texture::FilterMode)value); }
		}

		property Dimension dimension
		{
			Dimension get() { return (Dimension)((thomas::resource::Texture*)m_nativePtr)->GetDimension(); }
		}

		property int anistropicLevel
		{
			int get(){ return ((thomas::resource::Texture*)m_nativePtr)->GetAnistropicLevel(); }
			void set(int value) { return ((thomas::resource::Texture*)m_nativePtr)->SetAnistropicLevel(value); }
		}

	};
}
