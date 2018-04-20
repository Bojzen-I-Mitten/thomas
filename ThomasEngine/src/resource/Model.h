#pragma once
#pragma unmanaged
#include <thomas\resource\Model.h>
#include <thomas\utils\Primitives.h>
#pragma managed
#include "Resource.h"

namespace ThomasEditor
{
	public enum class PrimitiveType //could not typedef because no ToString
	{
		Sphere,
		Capsule,
		Cylinder,
		Cube,
		Plane,
		Quad
	};

	[DataContractAttribute]
	public ref class Model : public Resource
	{
	private:
		Model(thomas::resource::Model* nativePtr) : Resource(Utility::ConvertString(nativePtr->GetPath()), nativePtr){}
	public:
		Model(String^ path) : Resource(path, new thomas::resource::Model(Utility::ConvertString(path))) {};

		~Model()
		{
			if (m_nativePtr)
				delete m_nativePtr;
		}
		[OnDeserializedAttribute]
		void OnDeserialized(StreamingContext c)
		{

			m_nativePtr = new thomas::resource::Model(Utility::ConvertString(m_path));
		}
		
		static Model^ GetPrimitive(PrimitiveType type) { return gcnew Model(thomas::utils::Primitives::GetPrimitive((thomas::utils::Primitives::Type)type)); }
	};
}
