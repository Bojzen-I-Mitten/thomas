#pragma once
#pragma unmanaged
#include <thomas\object\component\physics\MeshCollider.h>
#pragma managed
#include "Collider.h"
#include "../../../resource/Model.h"
namespace ThomasEditor
{
	public ref class MeshCollider : public Collider
	{
	internal:

	public:
		MeshCollider() : Collider(new thomas::object::component::MeshCollider()) {}




		property Model^ mesh
		{
			Model^ get() {
				thomas::resource::Model* nativeModel = ((thomas::object::component::MeshCollider*)nativePtr)->GetMesh();
				Resource^ nativeResource = Resources::FindResourceFromNativePtr(nativeModel);
				if (nativeResource)
					return (Model^)nativeResource;
				else if (nativeModel)
					return gcnew Model(nativeModel);
				else
					return nullptr;
			}

			void set(Model^ value)
			{
				if (value)
					((thomas::object::component::MeshCollider*)nativePtr)->SetMesh((thomas::resource::Model*)value->m_nativePtr);
				else
					((thomas::object::component::MeshCollider*)nativePtr)->SetMesh(nullptr);
			}

		}
	};
}