#pragma once
#pragma unmanaged
#include "thomas\resource\Resource.h"
#pragma managed
#include "Resource.h"


namespace ThomasEditor
{
	public ref class Resources
	{
	internal:
		static System::Collections::Generic::Dictionary<String^, Resource^>^ resources = gcnew System::Collections::Generic::Dictionary<String^, Resource^>();
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

		static Resource^ Load(String^ path);
		
	};
}