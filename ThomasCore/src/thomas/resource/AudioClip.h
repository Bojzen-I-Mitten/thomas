#pragma once
#include "Resource.h"
#include <vector>
#include <memory>
namespace DirectX
{
	class SoundEffect;
	class SoundEffectInstance;
}
namespace thomas
{
	namespace resource
	{
		class AudioClip : public Resource
		{
		private:
			void OnChanged();
		public:
			AudioClip(std::string path);
			std::unique_ptr<DirectX::SoundEffectInstance> CreateInstance();
			~AudioClip();

		private:
			std::unique_ptr<DirectX::SoundEffect> m_data;
			std::vector<std::unique_ptr<DirectX::SoundEffectInstance>> m_instances;
		};
	}
}