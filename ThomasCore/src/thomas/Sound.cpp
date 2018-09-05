#include "Sound.h"
#include "Common.h"
#include <AtlBase.h>
#include <atlconv.h>

namespace thomas
{
	float Sound::s_masterVolume;
	float Sound::s_fxVolume;
	float Sound::s_musicVolume;
	std::unique_ptr<WaveBank> Sound::s_bank;
	std::unique_ptr<AudioEngine> Sound::s_audioEngine;
	std::map <std::string, std::unique_ptr<SoundEffect>> Sound::s_waves;

	bool Sound::Init()
	{
		//Win32 desktop
		CoInitialize(NULL);
		AUDIO_ENGINE_FLAGS aEFlags = DirectX::AudioEngine_Default;
#ifdef DEBUG
		aEFlags = aEFlags | DirectX::AudioEngine_Debug;
#endif
		s_audioEngine = std::make_unique<AudioEngine>(aEFlags);
		s_masterVolume = 0.5f;
		s_fxVolume = 0.5f;
		s_musicVolume = 0.5f;
		return true;
	}

	bool Sound::Play(const std::string & name, const float & volume = 1.f)
	{
		//Play a sound from the wavebank
		if (s_bank)
			s_bank->Play(name.c_str(), s_masterVolume * s_fxVolume * volume, 0.f, 0.f);
		else if (!s_waves.empty())
			s_waves.find(name)->second->Play(s_masterVolume * s_fxVolume * volume, 0.f, 0.f);
		else
			return false;

		return true;
	}

	std::unique_ptr<SoundEffectInstance> Sound::CreateInstance(const std::string & clipName, SOUND_EFFECT_INSTANCE_FLAGS flags = SoundEffectInstance_Default)
	{
		std::unique_ptr<SoundEffectInstance> instance;

		if (s_bank)
			instance = s_bank->CreateInstance(clipName.c_str(), flags);
		else if (!s_waves.empty())
			instance = s_waves.find(clipName)->second->CreateInstance(flags);
		else
		{
			LOG("No waves or wavebank loaded");
			return nullptr;
		}
		
		if (instance)
			return instance;
		else
		{
			LOG("Failed to find sound clip: " << clipName);
			return nullptr;
		}	
	}

	bool Sound::LoadWaveBank(const std::string & name)
	{
		//Create the wavebank
		try
		{
			s_bank = std::make_unique<WaveBank>(s_audioEngine.get(), CA2W(name.c_str()));
		}
		catch (std::exception ex)
		{
			LOG("Unable to load wavebank: " << name << ". Is the path correctly loaded?");
			s_bank = nullptr;
			return false;
		}
		return true;
	}

	std::unique_ptr<SoundEffect> Sound::LoadWave(const std::string & path)
	{
		try
		{
			return std::make_unique<SoundEffect>(s_audioEngine.get(), CA2W(path.c_str()));
		}
		catch (std::exception ex)
		{
			LOG("Unable to load wave: " << path << ". Probably invalid path and/or name");
			return nullptr;
		}
	}

	void Sound::Destroy()
	{
		s_audioEngine->Suspend();
	}

	void Sound::Update()
	{
		s_audioEngine->Update();
	}

	void Sound::SetMasterVolume(const float & volume)
	{
		s_masterVolume = volume;
	}

	void Sound::SetFxVolume(const float & volume)
	{
		s_fxVolume = volume;
	}

	void Sound::SetMusicVolume(const float & volume)
	{
		s_musicVolume = volume;
	}

	float Sound::GetMusicVolume()
	{
		return s_masterVolume * s_musicVolume;
	}
}