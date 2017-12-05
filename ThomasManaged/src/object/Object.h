#pragma once
#pragma unmanaged
#include <thomas\object\Object.h>
#pragma managed
#include <string>
#include <msclr\marshal_cppstd.h>

using namespace System;
using namespace System::Collections::Generic;
namespace ThomasEditor {
	public ref class Object
	{
		static List<Object^> s_objects;

	internal:
		thomas::object::Object* nativePtr;
	public:

		virtual void OnEnable() { nativePtr->OnEnable();}
		virtual void OnDisable() { nativePtr->OnDisable(); }
		virtual void OnDestroy() { nativePtr->OnDestroy(); }
		virtual void Start() { nativePtr->Start(); }
		virtual void Update() { nativePtr->Update(); }
		virtual void FixedUpdate() { nativePtr->FixedUpdate(); }
		virtual void LateUpdate() { nativePtr->LateUpdate(); }
		virtual void Render() { nativePtr->Render(); }
		thomas::Scene* GetScene() { return nativePtr->GetScene(); } //TODO

		static bool Destroy(Object^ object) {return thomas::object::Object::Destroy(object->nativePtr); };
		//static void Destroy(thomas::object:Scene* scene);
		static void Destroy() {return thomas::object::Object::Destroy(); }; //Maybe not
		static bool IsAlive(const Object^ object) { return thomas::object::Object::IsAlive(object->nativePtr); };
		bool Alive() { return nativePtr->Alive(); }

		Object()
		{
			s_objects.Add(this);
		}

		//Clone object

		//template<typename T>
		//static T* Instantiate(Scene* scene) {};


		//static std::vector<Object*> GetAllObjectsInScene(Scene* scene);

		static void Clean() { thomas::object::Object::Clean(); };
	};
}