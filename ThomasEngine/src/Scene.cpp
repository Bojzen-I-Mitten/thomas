#include "Scene.h"
#include "object\GameObject.h"
#include "object\Component.h"


namespace ThomasEditor
{
	void Scene::Play()
	{
		m_playing = true;
		for each(GameObject^ gObj in m_gameObjects)
		{
			gObj->Awake();
		}
	}

	void Scene::SaveSceneAs(Scene ^ scene, System::String ^ fullPath)
	{
		using namespace System::Runtime::Serialization;
		DataContractSerializerSettings^ serializserSettings = gcnew DataContractSerializerSettings();
		serializserSettings->KnownTypes = Component::GetAllComponentTypes()->ToArray();
		serializserSettings->PreserveObjectReferences = true;
		DataContractSerializer^ serializer = gcnew DataContractSerializer(Scene::typeid, serializserSettings);
		System::IO::FileInfo^ fi = gcnew System::IO::FileInfo(fullPath);
		fi->Directory->Create();
		Xml::XmlWriterSettings^ settings = gcnew Xml::XmlWriterSettings();
		settings->Indent = true;
		Xml::XmlWriter^ file = Xml::XmlWriter::Create(fullPath, settings);
		serializer->WriteObject(file, scene);
		file->Close();
		scene->m_savePath = fullPath;
	}

	void Scene::SaveScene(Scene ^ scene)
	{
		if(scene->m_savePath)
			SaveSceneAs(scene, scene->m_savePath);
	}

	Scene ^ Scene::LoadScene(System::String ^ fullPath)
	{

		s_loading = true;

		using namespace System::Runtime::Serialization;
		DataContractSerializerSettings^ serializserSettings = gcnew DataContractSerializerSettings();
		serializserSettings->KnownTypes = Component::GetAllComponentTypes()->ToArray();
		serializserSettings->PreserveObjectReferences = true;
		DataContractSerializer^ serializer = gcnew DataContractSerializer(Scene::typeid, serializserSettings);

		Xml::XmlReader^ file = Xml::XmlReader::Create(fullPath);
		Scene^ scene = (Scene^)serializer->ReadObject(file);
		file->Close();

		scene->PostLoad();
		s_loading = false;

		return scene;

	}

	void Scene::UnLoad()
	{
		for (int i = 0; i < m_gameObjects.Count; i++)
		{
			m_gameObjects[i]->Destroy();
			i--;
		}
		m_gameObjects.Clear();
	}

	void Scene::PostLoad()
	{
		for each(GameObject^ gObj in m_gameObjects)
		{
			gObj->PostLoad();
		}
	}

}

