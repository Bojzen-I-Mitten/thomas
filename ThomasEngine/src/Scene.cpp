#include "Scene.h"
#include "object\GameObject.h"
#include "object\Component.h"
#include "resource\Resources.h"
#include "ThomasManaged.h"
namespace ThomasEngine
{
	void Scene::Play()
	{
		String^ tempFile = System::IO::Path::Combine(Environment::GetFolderPath(Environment::SpecialFolder::LocalApplicationData), "thomas/scene.tds");
		SaveSceneAs(this, tempFile);
		m_playing = true;
		//for each(GameObject^ gObj in m_gameObjects)
		//{
		//	gObj->Awake();
		//}
	}

	Scene^ Scene::CurrentScene::get()
	{
		return s_currentScene;
	}

	void Scene::CurrentScene::set(Scene^ value)
	{
		ThomasWrapper::UnselectGameObjects();
		s_currentScene = value;
		if(Application::currentProject)
			Application::currentProject->currentScenePath = value->m_relativeSavePath;
	}


	void Scene::SaveSceneAs(Scene ^ scene, System::String ^ path)
	{
		using namespace System::Runtime::Serialization;
		DataContractSerializerSettings^ serializserSettings = gcnew DataContractSerializerSettings();

		auto list = Component::GetAllComponentTypes();
		list->Add(SceneResource::typeid);
		serializserSettings->KnownTypes = list->ToArray();
		serializserSettings->PreserveObjectReferences = true;
		serializserSettings->DataContractSurrogate = gcnew SceneSurrogate();
		DataContractSerializer^ serializer = gcnew DataContractSerializer(Scene::typeid, serializserSettings);
		System::IO::FileInfo^ fi = gcnew System::IO::FileInfo(path);
		fi->Directory->Create();
		Xml::XmlWriterSettings^ settings = gcnew Xml::XmlWriterSettings();
		settings->Indent = true;
		Xml::XmlWriter^ file = Xml::XmlWriter::Create(path, settings);
		serializer->WriteObject(file, scene);
		file->Close();
		
		if (Application::currentProject && scene->RelativeSavePath != path) {
			scene->m_relativeSavePath = path->Replace(Application::currentProject->assetPath + "\\", "");
			Application::currentProject->currentScenePath = scene->RelativeSavePath;
		}
			
	}

	void Scene::SaveScene(Scene ^ scene)
	{
		if(scene->RelativeSavePath)
			SaveSceneAs(scene, scene->RelativeSavePath);
	}

	Scene ^ Scene::LoadScene(System::String ^ fullPath)
	{

		s_loading = true;

		using namespace System::Runtime::Serialization;
		DataContractSerializerSettings^ serializserSettings = gcnew DataContractSerializerSettings();
		auto list = Component::GetAllComponentTypes();
		list->Add(SceneResource::typeid);
		serializserSettings->KnownTypes = list;
		serializserSettings->PreserveObjectReferences = true;
		serializserSettings->DataContractSurrogate = gcnew SceneSurrogate();
		DataContractSerializer^ serializer = gcnew DataContractSerializer(Scene::typeid, serializserSettings);
		Xml::XmlReader^ file = Xml::XmlReader::Create(fullPath);
		Scene^ scene = (Scene^)serializer->ReadObject(file);
		file->Close();

		scene->PostLoad();
		s_loading = false;
		if(Application::currentProject)
			scene->m_relativeSavePath = fullPath->Replace(Application::currentProject->assetPath + "\\", "");
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



	System::Type ^ Scene::SceneSurrogate::GetDataContractType(System::Type ^ type)
	{
		if (type->BaseType == Resource::typeid)
		{
			return SceneResource::typeid;
		}
		else
		{
			return type;
		}
	}

	System::Object ^ Scene::SceneSurrogate::GetObjectToSerialize(System::Object ^obj, System::Type ^targetType)
	{
		if (obj->GetType()->BaseType == Resource::typeid)
		{
			Resource^ resource = (Resource^)obj;
			return gcnew SceneResource(resource->GetPath());
		}
		return obj;
	}

	System::Object ^ Scene::SceneSurrogate::GetDeserializedObject(System::Object ^obj, System::Type ^targetType)
	{
		if (obj->GetType() == SceneResource::typeid)
		{
			SceneResource^ sceneResource = (SceneResource^)obj;
			if (sceneResource->path == "")
			{
				if (targetType == Material::typeid)
					return Material::StandardMaterial;
			}
			else
				return Resources::Load(sceneResource->path);
		}
		return obj;
	}

	System::Object ^ Scene::SceneSurrogate::GetCustomDataToExport(System::Reflection::MemberInfo ^memberInfo, System::Type ^dataContractType)
	{
		throw gcnew NotSupportedException("unused");
	}

	System::Object ^ Scene::SceneSurrogate::GetCustomDataToExport(System::Type ^clrType, System::Type ^dataContractType)
	{
		throw gcnew NotSupportedException("unused");
	}

	void Scene::SceneSurrogate::GetKnownCustomDataTypes(System::Collections::ObjectModel::Collection<System::Type ^> ^customDataTypes)
	{
		throw gcnew NotSupportedException("unused");
	}

	System::Type ^ Scene::SceneSurrogate::GetReferencedTypeOnImport(System::String ^typeName, System::String ^typeNamespace, System::Object ^customData)
	{
		throw gcnew NotSupportedException("unused");
	}

	System::CodeDom::CodeTypeDeclaration ^ Scene::SceneSurrogate::ProcessImportedType(System::CodeDom::CodeTypeDeclaration ^typeDeclaration, System::CodeDom::CodeCompileUnit ^compileUnit)
	{
		throw gcnew NotSupportedException("unused");
	}

}

