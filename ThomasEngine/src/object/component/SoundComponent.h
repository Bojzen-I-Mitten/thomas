#pragma once
#pragma unmanaged
#include <thomas\object\component\SoundComponent.h>
#include <thomas\object\component\Component.h>

#pragma managed
#include "../../resource/AudioClip.h"
#include "../../Utility.h"
#include "../Component.h"
#include "../../Input.h"

namespace ThomasEngine
{
	public ref class SoundComponent : public Component
	{
	private:
		AudioClip ^ m_clip;

	public:
		SoundComponent() : Component(new thomas::object::component::SoundComponent()) {}
		
		property AudioClip^ clip 
		{
			AudioClip^ get() 
			{
				return m_clip;
			}

			void set(AudioClip^ value)
			{
				m_clip = value;
				((thomas::object::component::SoundComponent*)nativePtr)->SetClip((thomas::resource::AudioClip*)value->m_nativePtr);
			}
		}

		property float Volume 
		{
			float get() 
			{
				return ((thomas::object::component::SoundComponent*)nativePtr)->GetVolume();
			}

			void set(float value)
			{
				((thomas::object::component::SoundComponent*)nativePtr)->SetVolume(value);
			}
		}
		
		property bool Looping 
		{
			bool get() 
			{
				return ((thomas::object::component::SoundComponent*)nativePtr)->IsLooping();
			}

			void set(bool value) 
			{
				return ((thomas::object::component::SoundComponent*)nativePtr)->SetLooping(value);
			}
		}

		bool Play() { return ((thomas::object::component::SoundComponent*)nativePtr)->Play(); }
		bool PlayOneShot(String^ name, float volume) { return ((thomas::object::component::SoundComponent*)nativePtr)->PlayOneShot(Utility::ConvertString(name), volume); }
		void Pause() { return ((thomas::object::component::SoundComponent*)nativePtr)->Pause(); }
		void Resume() { return ((thomas::object::component::SoundComponent*)nativePtr)->Resume(); }

		void Update() override
		{
			if (Input::GetKeyDown(Input::Keys::P))
			{
				Play();
				//((thomas::object::component::SoundComponent*)nativePtr)->Play();
			}
		}
	private:

	};
}