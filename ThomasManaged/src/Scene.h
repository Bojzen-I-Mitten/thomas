#pragma once

#pragma managed
#include <string>
#include <msclr\marshal_cppstd.h>

#using "PresentationFramework.dll"

namespace ThomasEditor {
	ref class GameObject;
	public ref class Scene
	{
		bool m_playing;
		static Scene^ s_currentScene;
		System::Object^ m_gameObjectsLock = gcnew System::Object();
		System::Collections::ObjectModel::ObservableCollection<GameObject^> m_gameObjects;
		System::String^ m_name;

	public:
		Scene(System::String^ name) {
			m_name = name;
			System::Windows::Data::BindingOperations::EnableCollectionSynchronization(%m_gameObjects, m_gameObjectsLock);
		}
		void Play();

		bool IsPlaying() { return m_playing; }

		property System::Collections::ObjectModel::ObservableCollection<GameObject^>^ GameObjects {
			System::Collections::ObjectModel::ObservableCollection<GameObject^>^ get() {
				return %m_gameObjects;
			}
		}

		static property Scene^ CurrentScene {
			Scene^ get() {
				return s_currentScene;
			}
			void set(Scene^ value) {
				s_currentScene = value;
			}
		}
	};
}
