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

	void Scene::SaveScene(Scene ^ scene, System::String ^ fullPath)
	{
		using namespace System::Xml::Serialization;

		XmlSerializer^ serializer = gcnew XmlSerializer(Scene::typeid, Component::GetAllComponentTypes()->ToArray());

		System::IO::TextWriter^ file = gcnew System::IO::StreamWriter(fullPath);
		serializer->Serialize(file, scene);

		file->Close();
	}

	Scene ^ Scene::LoadScene(System::String ^ fullPath)
	{
		using namespace System::Xml::Serialization;

		XmlSerializer^ serializer = gcnew XmlSerializer(Scene::typeid, Component::GetAllComponentTypes()->ToArray());

		System::IO::TextReader^ file = gcnew System::IO::StreamReader(fullPath);

		Scene^ scene = (Scene^)serializer->Deserialize(file);

		file->Close();

		return scene;
	}

	void Scene::UnLoad()
	{
		for each(GameObject^ gObj in m_gameObjects)
		{
			gObj->Destroy();
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

