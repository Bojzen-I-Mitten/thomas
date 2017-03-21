#include "SoundComponent.h"


namespace thomas
{
	namespace object
	{
		namespace component
		{
			SoundComponent::SoundComponent() : Component("SoundObject")
			{
				m_name == "";
				m_volume = 1;
				m_looping = true;
			}

			SoundComponent::~SoundComponent()
			{

			}




			bool SoundComponent::SetClip(std::string name)
			{
				m_instance = Sound::CreateInstance(name);
				if (m_instance)
				{
					m_instance->SetVolume(m_volume);
					m_name = name;
					return true;
				}
				else
					return false;
			}

			std::string SoundComponent::GetName()
			{
				return m_name;
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
				if (m_instance)
				{
					m_instance->Play(m_looping);
					return true;
				}
				else
					return false;
				
			}

			bool SoundComponent::PlayOneShot(std::string name, float volume)
			{
				return Sound::Play(name, volume);
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

			void SoundComponent::SetLooping(bool loop)
			{
				m_looping = loop;
			}

			bool SoundComponent::Looping()
			{
				return m_looping;
			}

		}
	}
}

