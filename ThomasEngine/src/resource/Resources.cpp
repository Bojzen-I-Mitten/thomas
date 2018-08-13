
#include "Model.h"
#include "AudioClip.h"
#include "Material.h"
#include "Shader.h"
#include "texture\Texture2D.h"
#include "Resources.h"
#include "../Scene.h"
namespace ThomasEditor
{
	Resource^ Resources::Load(String^ path)
	{
		String^ fullPath = System::IO::Path::GetFullPath(path);
		if (resources->ContainsKey(fullPath))
		{
			Resource^ obj = resources[fullPath];
			return obj;
		}
		else
		{
			Resource^ obj;
			AssetTypes type = GetResourceAssetType(path);
			switch (type)
			{
			case AssetTypes::MODEL:
				obj = gcnew Model(path);
				break;
			case AssetTypes::TEXTURE2D:
				obj = gcnew Texture2D(path);
				break;
			case AssetTypes::SCENE:
				break;
			case AssetTypes::SHADER:
				obj = gcnew Shader(path);
				break;
			case AssetTypes::MATERIAL:
				obj = Deserialize<Material^>(path);
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
				resources[fullPath] = obj;
			}
			return obj;
		}
		
	}
	Resources::AssetTypes Resources::GetResourceAssetType(Type ^ type)
	{
		
		if (type == AudioClip::typeid)
		{
			return AssetTypes::AUDIO_CLIP;
		}
		else if (type == Model::typeid)
		{
			return AssetTypes::MODEL;
		}
		else if (type == Material::typeid)
		{
			return AssetTypes::MATERIAL;
		}
		else if (type == Shader::typeid)
		{
			return AssetTypes::SHADER;
		}
		else if (type == Texture2D::typeid)
		{
			return AssetTypes::TEXTURE2D;
		}
		else
		{
			return AssetTypes::UNKNOWN;
		}
	}
	Resource ^ Resources::Find(String ^ path)
	{
		if (resources->ContainsKey(System::IO::Path::GetFullPath(path)))
		{
			return resources[System::IO::Path::GetFullPath(path)];
		}
		return nullptr;
	}
	void Resources::RenameResource(String ^ oldPath, String ^ newPath)
	{
		if (resources->ContainsKey(System::IO::Path::GetFullPath(oldPath)))
		{
			Object^ lock = Scene::CurrentScene->GetGameObjectsLock();

			System::Threading::Monitor::Enter(lock);
			Resource^ resource = resources[System::IO::Path::GetFullPath(oldPath)];
			resources->Remove(System::IO::Path::GetFullPath(oldPath));
			resources[System::IO::Path::GetFullPath(newPath)] = resource;
			if (resource)
				resource->Rename(newPath);

			System::Threading::Monitor::Exit(lock);
		}
	}
}