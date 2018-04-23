#pragma once
#include "Common.h"
#include "DirectXTK\SoundCommon.h"
#include <map>

using namespace DirectX;

namespace thomas
{
	class THOMAS_API Sound
	{
	public:
		static bool Init();
		static bool Play(const std::string & name, const float & volume);
		static std::unique_ptr<SoundEffectInstance> CreateInstance(const std::string & clipName, SOUND_EFFECT_INSTANCE_FLAGS flags);
		static bool LoadWaveBank(const std::string & name);
		static std::unique_ptr<SoundEffect> LoadWave(const std::string & path);
		static void Destroy();
		static void Update();

	public:
		static void SetMasterVolume(const float & volume);
		static void SetFxVolume(const float & volume);
		static void SetMusicVolume(const float & volume);

	public:
		static float GetMusicVolume();

	private:
		static float s_masterVolume;
		static float s_fxVolume;
		static float s_musicVolume;

	private:
		static std::unique_ptr<WaveBank> s_bank;
		static std::unique_ptr<AudioEngine> s_audioEngine;
		static std::map<std::string, std::unique_ptr<SoundEffect>> s_waves;
	};
}