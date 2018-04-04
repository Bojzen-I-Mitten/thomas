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
		static bool s_loading = false;
		static Scene^ s_currentScene;
		System::Object^ m_gameObjectsLock = gcnew System::Object();
		System::Collections::ObjectModel::ObservableCollection<GameObject^> m_gameObjects;
		System::String^ m_name;
		System::String^ m_savePath;

		Scene() {
			m_name = "New Scene";
			System::Windows::Data::BindingOperations::EnableCollectionSynchronization(%m_gameObjects, m_gameObjectsLock);
			m_gameObjects.CollectionChanged += sceneChanged;
		}
	public:

		static System::Collections::Specialized::NotifyCollectionChangedEventHandler^ sceneChanged;

		Scene(System::String^ name) {
			m_name = name;
			System::Windows::Data::BindingOperations::EnableCollectionSynchronization(%m_gameObjects, m_gameObjectsLock);
			m_gameObjects.CollectionChanged += sceneChanged;
		}
		void Play();
		void Stop() { m_playing = false; }

		bool IsPlaying() { return m_playing; }

		System::Object^ GetGameObjectsLock()
		{
			return m_gameObjectsLock;
		}

		property System::String^ Name {
			System::String^ get() { return m_name; }
			void set(System::String^ value) { m_name = value; }
		}

		property bool HasFile {
			bool get() { return m_savePath != nullptr; }
		}

		
		property System::Collections::ObjectModel::ObservableCollection<GameObject^>^ GameObjects {
			System::Collections::ObjectModel::ObservableCollection<GameObject^>^ get() {
				return %m_gameObjects;
			}
		}



		static void SaveSceneAs(Scene^ scene, System::String^ fullPath);
		static void SaveScene(Scene^ scene);

		static Scene^ LoadScene(System::String^ fullPath);
		
		static bool IsLoading()
		{
			return s_loading;
		}

		static void RestartCurrentScene();


		void UnLoad();
		void PostLoad();

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
