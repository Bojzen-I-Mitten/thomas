#include "AudioClip.h"
#include "../Sound.h"

namespace thomas
{
	namespace resource
	{
		AudioClip::AudioClip(std::string name) : Resource(name)
		{
			Sound::LoadWave(name);
			Sound::LoadWaveBank(name);
		}
		AudioClip::~AudioClip()
		{
		}
	}
}
