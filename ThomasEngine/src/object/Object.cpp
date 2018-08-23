#include "Object.h"
#include "GameObject.h"
#include "component\Transform.h"
#include "../Scene.h"
namespace ThomasEngine
{
	GameObject ^ Object::Instantiate(GameObject ^ original)
	{
		System::IO::Stream^ serialized = SerializeGameObject(original);
		GameObject^ clone = DeSerializeGameObject(serialized);
		Scene::CurrentScene->GameObjects->Add(clone);
		return clone;
	}

	GameObject ^ Object::Instantiate(GameObject ^ original, Transform^ parent)
	{
		GameObject^ clone = Instantiate(original);
		clone->transform->parent = parent;
		return clone;
	}

	GameObject ^ Object::Instantiate(GameObject ^ original, Vector3 position, Quaternion rotation)
	{
		GameObject^ clone = Instantiate(original);
		clone->transform->position = position;
		clone->transform->rotation = rotation;
		return clone;
	}

	GameObject ^ Object::Instantiate(GameObject ^ original, Vector3 position, Quaternion rotation, Transform^ parent)
	{
		GameObject^ clone = Instantiate(original, parent);
		clone->transform->position = position;
		clone->transform->rotation = rotation;
		return clone;
	}

	System::IO::Stream^ Object::SerializeGameObject(GameObject ^ gObj)
	{
		using namespace System::Runtime::Serialization;
		DataContractSerializerSettings^ serializerSettings = gcnew DataContractSerializerSettings();
		auto list = Component::GetAllComponentTypes();
		list->Add(SceneResource::typeid);
		serializerSettings->KnownTypes = list;
		serializerSettings->PreserveObjectReferences = true;
		serializerSettings->DataContractSurrogate = gcnew Scene::SceneSurrogate();
		DataContractSerializer^ serializer = gcnew DataContractSerializer(GameObject::typeid, serializerSettings);
		
		System::IO::Stream^ stream = gcnew System::IO::MemoryStream();

		Xml::XmlWriter^ writer = Xml::XmlWriter::Create(stream);

		serializer->WriteObject(writer, gObj);

		Xml::XmlReader^ file = Xml::XmlReader::Create(stream);
		return stream;
	}

	GameObject ^ Object::DeSerializeGameObject(System::IO::Stream ^ stream)
	{
		using namespace System::Runtime::Serialization;
		DataContractSerializerSettings^ serializerSettings = gcnew DataContractSerializerSettings();
		auto list = Component::GetAllComponentTypes();
		list->Add(SceneResource::typeid);
		serializerSettings->KnownTypes = list;
		serializerSettings->PreserveObjectReferences = true;
		serializerSettings->DataContractSurrogate = gcnew Scene::SceneSurrogate();
		DataContractSerializer^ serializer = gcnew DataContractSerializer(GameObject::typeid, serializerSettings);

		Xml::XmlReader^ reader = Xml::XmlReader::Create(stream);

		GameObject^ gObj = (GameObject^)serializer->ReadObject(reader);
		return gObj;

	}

}
