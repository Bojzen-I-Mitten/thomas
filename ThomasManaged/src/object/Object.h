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
		thomas::object::Object* nativePtr;
	protected:
		Object(thomas::object::Object* ptr) {
			nativePtr = ptr;
		}
	public:
		Object(String^ type) {
			nativePtr = new thomas::object::Object(msclr::interop::marshal_as<std::string>(type));
		}

		virtual void OnEnable() { nativePtr->OnEnable();}
		virtual void OnDisable() { nativePtr->OnDisable(); }
		virtual void OnDestroy() { nativePtr->OnDestroy(); }
		virtual void Start() { nativePtr->Start(); }
		virtual void Update() { nativePtr->Update(); }
		virtual void FixedUpdate() { nativePtr->FixedUpdate(); }
		virtual void LateUpdate() { nativePtr->LateUpdate(); }
		virtual void Render() { nativePtr->Render(); }
		virtual void SetActive(bool active) { nativePtr->SetActive(active); };
		virtual bool GetActive() { return nativePtr->GetActive(); }
		String^ GetType() { return gcnew String(nativePtr->GetType().c_str()); };
		thomas::Scene* GetScene() { return nativePtr->GetScene(); } //TODO

		static bool Destroy(Object^ object) {return thomas::object::Object::Destroy(object->nativePtr); };
		//static void Destroy(thomas::object:Scene* scene);
		static void Destroy() {return thomas::object::Object::Destroy(); }; //Maybe not
		static bool IsAlive(const Object^ object) { return thomas::object::Object::IsAlive(object->nativePtr); };
		bool Alive() { return nativePtr->Alive(); }
		//Clone object

		//template<typename T>
		//static T* Instantiate(Scene* scene) {};

		static List<Object^>^ GetObjects() 
		{
				
			std::vector<thomas::object::Object*> nativeObjects = thomas::object::Object::GetObjects();
				
			List<Object^>^ managedObjects = gcnew List<Object^>(nativeObjects.size());

			for (thomas::object::Object* nativeObject : nativeObjects)
			{
				managedObjects->Add(gcnew Object(nativeObject));
			}
			return managedObjects;
		};
		static Object^ GetObjectByType(String^ type) {
			return gcnew Object(thomas::object::Object::GetObjectByType(msclr::interop::marshal_as<std::string>(type)));
		}

		//static std::vector<Object*> GetAllObjectsInScene(Scene* scene);

		static void Clean() { thomas::object::Object::Clean(); };
	};
}