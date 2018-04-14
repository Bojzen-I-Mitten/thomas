#pragma once
#pragma unmanaged
#include "thomas\resource\Resource.h"
#pragma managed
#include "Resource.h"

using namespace System::Collections::Generic;
using namespace System::Linq;
using namespace System::Threading;
namespace ThomasEditor
{
	public ref class Resources
	{
	internal:
		static Object^ resourceLock = gcnew Object();
		static Dictionary<String^, Resource^>^ resources = gcnew Dictionary<String^, ThomasEditor::Resource^>();

		generic<typename T>
		where T : Resource
		static T Deserialize(String^ path)
		{
			Monitor::Enter(resourceLock);
			using namespace System::Runtime::Serialization;
			DataContractSerializerSettings^ serializserSettings = gcnew DataContractSerializerSettings();
			serializserSettings->PreserveObjectReferences = true;
			serializserSettings->KnownTypes = System::Reflection::Assembly::GetAssembly(Resource::typeid)->ExportedTypes;
			DataContractSerializer^ serializer = gcnew DataContractSerializer(T::typeid, serializserSettings);

			Xml::XmlReader^ file = Xml::XmlReader::Create(path);
			T resource = (T)serializer->ReadObject(file);
			file->Close();
			Monitor::Exit(resourceLock);
			return resource;
		}
	public:

		enum class AssetTypes
		{
			MODEL,
			TEXTURE2D,
			TEXTURE3D,
			SCENE,
			SHADER,
			MATERIAL,
			SCRIPT,
			AUDIO_CLIP,
			UNKNOWN
		};

		static void SaveResource(Resource^ resource, String^ path)
		{
			Monitor::Enter(resourceLock);
			path = "..\\Data\\" + path;
			using namespace System::Runtime::Serialization;
			DataContractSerializerSettings^ serializserSettings = gcnew DataContractSerializerSettings();
			serializserSettings->PreserveObjectReferences = true;
			serializserSettings->KnownTypes = System::Reflection::Assembly::GetAssembly(Resource::typeid)->ExportedTypes;
			DataContractSerializer^ serializer = gcnew DataContractSerializer(resource->GetType(), serializserSettings);
			System::IO::FileInfo^ fi = gcnew System::IO::FileInfo(path);

			fi->Directory->Create();
			Xml::XmlWriterSettings^ settings = gcnew Xml::XmlWriterSettings();
			settings->Indent = true;
			Xml::XmlWriter^ file = Xml::XmlWriter::Create(path, settings);
			resource->m_path = path;
			serializer->WriteObject(file, resource);
			file->Close();
			resources[System::IO::Path::GetFullPath(path)] = resource;
			Monitor::Exit(resourceLock);
		}

		static AssetTypes GetResourceAssetType(Type^ type);

		static AssetTypes GetResourceAssetType(String^ path)
		{
			String^ extension = System::IO::Path::GetExtension(path)->Remove(0, 1);
			if (extension == "fx")
			{
				return AssetTypes::SHADER;
			}
			else if (extension == "tds")
			{
				return AssetTypes::SCENE;
			}
			else if (extension == "wav")
			{
				return AssetTypes::AUDIO_CLIP;
			}
			else if (extension == "obj")
			{
				return AssetTypes::MODEL;
			}
			else if (extension == "mat")
			{
				return AssetTypes::MATERIAL;
			}
			else if (extension == "bmp" || extension == "jpg" || extension == "png" || extension == "gif" || extension == "tif")
			{
				return AssetTypes::TEXTURE2D;
			}
			else
			{
				return AssetTypes::UNKNOWN;
			}
		}

		generic<typename T>
		where T : Resource
		static T Load(String^ path) 
		{
			if (resources->ContainsKey(System::IO::Path::GetFullPath(path)))
			{
				Resource^ obj = resources[System::IO::Path::GetFullPath(path)];
				if (obj->GetType() == T::typeid)
					return (T)obj;
				else
				{
					return T();
					//error blablablabla.
				}
			}
			else
			{
				T resource = (T)Activator::CreateInstance(T::typeid, path);
				resources[System::IO::Path::GetFullPath(path)] = resource;
				return resource;
			}
		}

		generic<typename T>
		where T : Resource
		static List<T>^ GetResourcesOfType()
		{
			return (List<T>^)Enumerable::OfType<T>(resources->Values);
		}

		static List<Resource^>^ GetResourcesOfType(Type^ type)
		{
			List<Resource^>^ list = gcnew List<Resource^>();
			for each (Resource^ resource in resources->Values)
			{
				if (resource->GetType() == type)
					list->Add(resource);
			}
			return list;
		}

		static Resource^ Load(String^ path);
		
		static Resource^ Find(String^ path);

		static void RenameResource(String^ oldPath, String^ newPath);

		static void UnloadAll()
		{
			for each(String^ resource in resources->Keys)
			{
				resources[resource]->~Resource();
			}
		}
	};
}