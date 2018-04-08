
#include "Model.h"
#include "AudioClip.h"
#include "Resources.h"
namespace ThomasEditor
{
	Resource^ Resources::Load(String^ path)
	{
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
					obj = gcnew Model(path);
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
		else
		{
			return AssetTypes::UNKNOWN;
		}
	}
}