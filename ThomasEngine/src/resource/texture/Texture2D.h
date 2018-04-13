#pragma once
#pragma unmanaged
#include <thomas\resource\texture\Texture2D.h>
#pragma managed

#include "Texture.h"

namespace ThomasEditor
{
	[DataContractAttribute]
	public ref class Texture2D : public Texture
	{
	public:

	internal:
		Texture2D(String^ path) : Texture(path, new thomas::resource::Texture2D(Utility::ConvertString(path))) {};
		Texture2D(int width, int height, bool mipMap, bool linear) : Texture("", new thomas::resource::Texture2D(width, height, mipMap, linear)) {};
		Texture2D(thomas::resource::Texture2D* nativePtr) : Texture("", nativePtr) {};
	public:

	};
}
