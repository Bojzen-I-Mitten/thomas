#include "AudioClip.h"
#include "../Sound.h"

namespace thomas
{
	namespace resource
	{
		AudioClip::AudioClip(std::string path) : Resource(path)
		{

			m_data = std::unique_ptr<DirectX::SoundEffect>(Sound::LoadWave(path));
		}
		std::unique_ptr<DirectX::SoundEffectInstance> AudioClip::CreateInstance()
		{
			return m_data->CreateInstance();
		}
		AudioClip::~AudioClip()
		{
			m_data.release();
		}
	}
}
