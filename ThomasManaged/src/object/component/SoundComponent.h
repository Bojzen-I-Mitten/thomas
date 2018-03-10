#pragma once
#pragma unmanaged
#include <thomas\object\component\SoundComponent.h>
#include <thomas\object\component\Component.h>

#pragma managed
#include <string>
#include <msclr\marshal_cppstd.h>
#include "../../Utility.h"
#include "../Component.h"


namespace ThomasEditor
{
	public ref class SoundComponent : public Component
	{
		SoundComponent() : Component(new thomas::object::component::SoundComponent()) {}
	public:

		bool SetClip(String^ name) { return ((thomas::object::component::SoundComponent*)nativePtr)->SetClip(Utility::ConvertString(name)); }
		String^ GetName() {	return Utility::ConvertString(((thomas::object::component::SoundComponent*)nativePtr)->GetName()); }
		bool SetVolume(float volume) { return ((thomas::object::component::SoundComponent*)nativePtr)->SetVolume(volume); }
		float GetVolume() { return ((thomas::object::component::SoundComponent*)nativePtr)->GetVolume(); }
		bool Play() { return ((thomas::object::component::SoundComponent*)nativePtr)->Play(); }
		bool PlayOneShot(String^ name, float volume) { return ((thomas::object::component::SoundComponent*)nativePtr)->PlayOneShot(Utility::ConvertString(name), volume); }
		void Pause() { return ((thomas::object::component::SoundComponent*)nativePtr)->Pause(); }
		void Resume() { return ((thomas::object::component::SoundComponent*)nativePtr)->Resume(); }
		void SetLooping(bool loop) { return ((thomas::object::component::SoundComponent*)nativePtr)->SetLooping(loop); }
		bool IsLooping() { return ((thomas::object::component::SoundComponent*)nativePtr)->IsLooping(); }
	private:

	};

}