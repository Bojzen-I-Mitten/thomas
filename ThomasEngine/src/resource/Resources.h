#pragma once
#pragma unmanaged
#include "thomas\resource\Resource.h"
#pragma managed
#include "Resource.h"

using namespace System::Collections::Generic;
using namespace System::Linq;
namespace ThomasEditor
{
	public ref class Resources
	{
	internal:
		static Dictionary<String^, Resource^>^ resources = gcnew Dictionary<String^, ThomasEditor::Resource^>();
	public:

		enum class AssetTypes
		{
			MODEL,
			TEXTURE,
			SCENE,
			SHADER,
			MATERIAL,
			SCRIPT,
			AUDIO_CLIP,
			UNKNOWN
		};

		static void SaveResource(Resource^ resource, String^ path)
		{
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

			resources[path] = resource;
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
			else
			{
				return AssetTypes::UNKNOWN;
			}
		}

		generic<typename T>
		where T : Resource
		static T Load(String^ path) 
		{
			if (resources->ContainsKey(path))
			{
				Resource^ obj = resources[path];
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
				resources[path] = resource;
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
		
	};
}