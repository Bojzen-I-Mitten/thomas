#pragma once

#include "Component.h"
#include <memory>

namespace DirectX {
	class SoundEffectInstance;
}
namespace thomas
{
	namespace object
	{
		namespace component
		{
			class SoundComponent : public Component
			{
			public:
				SoundComponent();
				~SoundComponent();
				bool SetClip(std::string name);
				std::string GetName();
				bool SetVolume(float volume);
				float GetVolume();
				bool Play();
				bool PlayOneShot(std::string name, float volume);
				void Pause();
				void Resume();
				void SetLooping(bool loop);
				bool IsLooping();
			private:
				std::string m_name;
				float m_volume;
				bool m_looping;
				std::unique_ptr<DirectX::SoundEffectInstance> m_instance;
			};
		}
	}
}