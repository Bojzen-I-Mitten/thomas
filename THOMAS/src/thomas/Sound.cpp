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
	std::map < std::string, std::unique_ptr<DirectX::SoundEffect>> Sound::s_waves;

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

	bool Sound::Play(std::string name, float volume = 1.f)
	{
		if (s_bank)
		{
			s_bank->Play(name.c_str(), s_masterVolume * s_fxVolume * volume, 0.0f, 0.0f);
		}
		else if (!s_waves.empty())
		{
			s_waves.find(name)->second->Play(s_masterVolume * s_fxVolume * volume, 0.f, 0.f);
		}
		else
		{
			return false;
		}
		return true;
	}

	std::unique_ptr<DirectX::SoundEffectInstance> Sound::CreateInstance(std::string clipName, DirectX::SOUND_EFFECT_INSTANCE_FLAGS flags = DirectX::SoundEffectInstance_Default)
	{
		std::unique_ptr<DirectX::SoundEffectInstance> instance = NULL;

		if (s_bank)
		{
			instance = s_bank->CreateInstance(clipName.c_str(), flags);
		}
		else if (!s_waves.empty())
		{
			instance = s_waves.find(clipName)->second->CreateInstance(flags);
		}
		else
		{
			LOG("No waves or wavebank loaded");
			return NULL;
		}
		
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
			LOG("Unable to load wavebank: " << name << ". Probably invalid path and/or name");
			s_bank = NULL;
			return false;
		}
		return true;
	}

	bool Sound::LoadWave(std::string name)
	{
		try
		{
			s_waves.emplace(name, std::unique_ptr<DirectX::SoundEffect>(new DirectX::SoundEffect(s_audioEngine.get(), CA2W(name.c_str()))));
		}
		catch (std::exception ex)
		{
			LOG("Unable to load wave: " << name << ". Probably invalid path and/or name");
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