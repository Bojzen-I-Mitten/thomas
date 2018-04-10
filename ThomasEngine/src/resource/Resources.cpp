
#include "Model.h"
#include "AudioClip.h"
#include "Material.h"
#include "Shader.h"
#include "Resources.h"
namespace ThomasEditor
{
	Resource^ Resources::Load(String^ path)
	{
		{
			if (resources->ContainsKey(System::IO::Path::GetFullPath(path)))
			{
				Resource^ obj = resources[System::IO::Path::GetFullPath(path)];
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
				case AssetTypes::TEXTURE:
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
					resources[System::IO::Path::GetFullPath(path)] = obj;
				}
				return obj;
			}
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
		else
		{
			return AssetTypes::UNKNOWN;
		}
	}
}