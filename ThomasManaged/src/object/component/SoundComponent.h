#pragma once
#pragma unmanaged
#include <thomas\object\component\SoundComponent.h>
#include <thomas\object\component\Component.h>

#pragma managed
#include <string>
#include <msclr\marshal_cppstd.h>
#include "../../Utility.h"
#include "../Component.h"
#include "../../Input.h"

namespace ThomasEditor
{
	public ref class SoundComponent : public Component
	{
	public:
		SoundComponent() : Component(new thomas::object::component::SoundComponent()) {}
		
		property String^ SoundClip {
			String^ get() {
				return Utility::ConvertString(((thomas::object::component::SoundComponent*)nativePtr)->GetName());
			}
			void set(String^ value)
			{
				((thomas::object::component::SoundComponent*)nativePtr)->SetClip(Utility::ConvertString(value));
			}
		}
		property float Volume {
			float get() {
				return ((thomas::object::component::SoundComponent*)nativePtr)->GetVolume();
			}
			void set(float value)
			{
				((thomas::object::component::SoundComponent*)nativePtr)->SetVolume(value);
			}
		}
		
		property bool Looping {
			bool get() {
				return ((thomas::object::component::SoundComponent*)nativePtr)->IsLooping();
			}
			void set(bool value) {
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