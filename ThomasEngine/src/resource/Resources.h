#pragma once
#pragma unmanaged
#include "thomas\resource\Resource.h"
#pragma managed
#include <msclr\marshal_cppstd.h>
#include "Resource.h"
#include "AudioClip.h"
using namespace System;
using namespace System::Collections::Generic;
using namespace System::ComponentModel;

namespace ThomasEditor
{

	
	public ref class Resources
	{
	internal:
		static Dictionary<String^, Resource^>^ resources = gcnew Dictionary<String^, Resource^>();
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

		static Resource^ Load(String^ path)
		{
			if (resources->ContainsKey(path))
			{
				Resource^ obj = resources[path];
				return obj;
			}
			else
			{
				Resource^ obj;
				AssetTypes type = GetResourceAssetType(path);
				switch (type)
				{
				case AssetTypes::MODEL:
					break;
				case AssetTypes::TEXTURE:
					break;
				case AssetTypes::SCENE:
					break;
				case AssetTypes::SHADER:
					break;
				case AssetTypes::MATERIAL:
					break;
				case AssetTypes::SCRIPT:
					break;
				case AssetTypes::AUDIO_CLIP:
					obj = gcnew AudioClip(path);
					break;
				case AssetTypes::UNKNOWN:
					break;
				default:
					break;
				}
				if (obj != nullptr)
				{
					resources[path] = obj;
				}
				return obj;
			}
		}
	};
}