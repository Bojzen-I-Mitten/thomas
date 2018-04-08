#include "Scene.h"
#include "object\GameObject.h"
#include "object\Component.h"


namespace ThomasEditor
{
	void Scene::Play()
	{
		String^ tempFile = System::IO::Path::Combine(Environment::GetFolderPath(Environment::SpecialFolder::LocalApplicationData), "thomas/scene.tds");
		SaveSceneAs(this, tempFile);
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
		scene->m_savePath = fullPath;
		return scene;

	}

	void Scene::RestartCurrentScene()
	{
		Object^ lock = Scene::CurrentScene->GetGameObjectsLock();
		String^ tempFile = System::IO::Path::Combine(Environment::GetFolderPath(Environment::SpecialFolder::LocalApplicationData), "thomas/scene.tds");
		Monitor::Enter(lock);
		Scene::CurrentScene->UnLoad();
		Scene::CurrentScene = Scene::LoadScene(tempFile);
		System::IO::File::Delete(tempFile);
		Monitor::Exit(lock);
	}

	void Scene::UnLoad()
	{
		for (int i = 0; i < m_gameObjects.Count; i++)
		{
			m_gameObjects[i]->Destroy();
			i--;
		}
		m_gameObjects.Clear();
		m_gameObjects.CollectionChanged -= sceneChanged;
	}

	void Scene::PostLoad()
	{
		for each(GameObject^ gObj in m_gameObjects)
		{
			gObj->PostLoad();
		}
	}

}

