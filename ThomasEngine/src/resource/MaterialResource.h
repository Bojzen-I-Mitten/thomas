#pragma once
#pragma managed
#include "Resource.h"
namespace ThomasEditor
{
	ref class Material;
	[DataContractAttribute]
	public ref class MaterialResource : public Resource
	{
	private:
		[DataMemberAttribute]
		Material^ material;
	public:
		MaterialResource(String^ path) : Resource(path, nullptr) {};
		MaterialResource(Material^ mat) : Resource("", nullptr)
		{
			material = mat;
		}
	};
}
