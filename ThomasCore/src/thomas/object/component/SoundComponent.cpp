#include "SoundComponent.h"
#include "../../Sound.h"
#include "../../resource/AudioClip.h"
namespace thomas
{
	namespace object
	{
		namespace component
		{
			SoundComponent::SoundComponent()
			{
				m_name == "";
				m_volume = 1;
				m_looping = true;
			}

			SoundComponent::~SoundComponent()
			{

			}

			bool SoundComponent::SetClip(resource::AudioClip* clip)
			{
				m_clip = clip;
				m_instance = clip->CreateInstance();
				if (m_instance)
				{
					m_instance->SetVolume(m_volume);
					//m_name = name;
					return true;
				}
				else
				{
					////Sound::LoadWaveBank(name);
					//Sound::LoadWave(name);

					//m_instance = Sound::CreateInstance(name);
					//m_instance->SetVolume(m_volume);
					//m_name = name;
					//return true;
					//return false;
				}
				return false;
			}

			resource::AudioClip* SoundComponent::GetClip()
			{
				return m_clip;
			}

			bool SoundComponent::SetVolume(float volume)
			{
				if (volume > 5)
					return false;
				m_volume = volume;
				if (m_instance)
				{
					m_instance->SetVolume(volume*Sound::GetMusicVolume());
					
				}
				return true;
			}

			float SoundComponent::GetVolume()
			{
				return m_volume;
			}

			bool SoundComponent::Play()
			{
				m_instance = m_clip->CreateInstance();
				if (m_instance)
				{
					m_instance->Play(m_looping);
					return true;
				}
				else
					return false;
				
			}

			bool SoundComponent::PlayOneShot(float volume)
			{
				m_clip->Play();
				return true;
			}

			void SoundComponent::Pause()
			{
				if (m_instance)
				{
					m_instance->Pause();
				}
			}

			void SoundComponent::Resume()
			{
				if (m_instance)
				{
					m_instance->Resume();
				}
			}

			void SoundComponent::Stop()
			{
				if (m_instance)
				{
					m_instance->Stop();
				}
			}

			void SoundComponent::SetLooping(bool loop)
			{
				m_looping = loop;
			}

			bool SoundComponent::IsLooping()
			{
				return m_looping;
			}
		}
	}
}
