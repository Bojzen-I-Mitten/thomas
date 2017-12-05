#include "Sound.h"
#include <string>
#include <AtlBase.h>
#include <atlconv.h>

namespace thomas
{
	float Sound::s_masterVolume;
	float Sound::s_fxVolume;
	float Sound::s_musicVolume;
	std::unique_ptr<DirectX::WaveBank> Sound::s_bank;
	std::unique_ptr<DirectX::AudioEngine> Sound::s_audioEngine;

	bool Sound::Init()
	{
		//because win32 desktop app
		CoInitialize(nullptr);
		DirectX::AUDIO_ENGINE_FLAGS aEFlags = DirectX::AudioEngine_Default;
#ifdef DEBUG
		aEFlags = aEFlags | DirectX::AudioEngine_Debug;
#endif // DEBUG

		s_audioEngine = std::make_unique<DirectX::AudioEngine>(aEFlags);
		s_masterVolume = 0.5f;
		s_fxVolume = 0.5f;
		s_musicVolume = 0.5f;
		s_bank = NULL;
		return true;
	}

	bool Sound::Play(std::string name, float volume)
	{
		if(s_bank)
			s_bank->Play(name.c_str(), s_masterVolume * s_fxVolume * volume, 0.0f, 0.0f);
		return true;
	}

	std::unique_ptr<DirectX::SoundEffectInstance> Sound::CreateInstance(std::string clipName)
	{
		if (!s_bank)
			return NULL;
		std::unique_ptr<DirectX::SoundEffectInstance> instance = s_bank->CreateInstance(clipName.c_str());
		if (instance)
		{
			return instance;
		}
		else
		{
			LOG("Failed to find sound clip: " << clipName);
			return NULL;
		}
		
	}



	void Sound::SetMasterVolume(float volume)
	{
		s_masterVolume = volume;
	}
	void Sound::SetFxVolume(float volume)
	{
		s_fxVolume = volume;
	}
	void Sound::SetMusicVolume(float volume)
	{
		s_musicVolume = volume;
	}
	float Sound::GetMusicVolume()
	{
		return s_masterVolume*s_musicVolume;
	}
	bool Sound::LoadWaveBank(std::string name)
	{
		try
		{
			s_bank = std::make_unique<DirectX::WaveBank>(s_audioEngine.get(), CA2W(name.c_str()));
		}
		catch (std::exception ex)
		{
			LOG("Unable to load wavebank: " << name << " .Probably invalid path and/or name");
			s_bank = NULL;
			return false;
		}
		return true;
	}
	void Sound::Destroy()
	{
		s_audioEngine->Suspend();
	}
	void Sound::Update()
	{
		s_audioEngine->Update();
	}
}