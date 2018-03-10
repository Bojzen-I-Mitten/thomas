#pragma once
#include "common.h"
#include <string>
#include <vector>
#include "DirectXTK/SoundCommon.h"
#include <map>


namespace thomas
{
	class THOMAS_API Sound
	{
	public:
	public:
		static bool Init();
		static bool Play(std::string name, float volume);
		static std::unique_ptr<DirectX::SoundEffectInstance> CreateInstance(std::string clipName, DirectX::SOUND_EFFECT_INSTANCE_FLAGS flags = DirectX::SoundEffectInstance_Default);
		static void SetMasterVolume(float volume);
		static void SetFxVolume(float volume);
		static void SetMusicVolume(float volume);
		static float GetMusicVolume();
		static bool LoadWaveBank(std::string name);
		static bool LoadWave(std::string name);
		static void Destroy();
		static void Update();
	private:
		static float s_masterVolume;
		static float s_fxVolume;
		static float s_musicVolume;
		static std::unique_ptr<DirectX::WaveBank> s_bank;
		static std::unique_ptr<DirectX::AudioEngine> s_audioEngine;
		static std::map<std::string, std::unique_ptr<DirectX::SoundEffect>> s_waves;
	};
}