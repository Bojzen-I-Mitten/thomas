#pragma once

#pragma managed
#include <string>
#include <msclr\marshal_cppstd.h>

#using "PresentationFramework.dll"


namespace ThomasEngine {
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

		property System::String^ SavePath {
			System::String^ get() { return m_savePath; }
			void set(System::String^ value) { m_savePath = value; }
		}

		System::Object^ GetGameObjectsLock()
		{
			return m_gameObjectsLock;
		}

		property System::String^ Name {
			System::String^ get() { return m_name; }
			void set(System::String^ value) { m_name = value; }
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
			Scene^ get();
			void set(Scene^ value);
		}

	public:
		ref class SceneSurrogate : System::Runtime::Serialization::IDataContractSurrogate
		{
			public:
				virtual System::Type ^ GetDataContractType(System::Type ^type);
				virtual System::Object ^ GetObjectToSerialize(System::Object ^obj, System::Type ^targetType);
				virtual System::Object ^ GetDeserializedObject(System::Object ^obj, System::Type ^targetType);
				virtual System::Object ^ GetCustomDataToExport(System::Reflection::MemberInfo ^memberInfo, System::Type ^dataContractType);
				virtual System::Object ^ GetCustomDataToExport(System::Type ^clrType, System::Type ^dataContractType);
				virtual void GetKnownCustomDataTypes(System::Collections::ObjectModel::Collection<System::Type ^> ^customDataTypes);
				virtual System::Type ^ GetReferencedTypeOnImport(System::String ^typeName, System::String ^typeNamespace, System::Object ^customData);
				virtual System::CodeDom::CodeTypeDeclaration ^ ProcessImportedType(System::CodeDom::CodeTypeDeclaration ^typeDeclaration, System::CodeDom::CodeCompileUnit ^compileUnit);
		};

		
	};
	[System::Runtime::Serialization::DataContractAttribute]
	ref class SceneResource
	{
	public:
		[System::Runtime::Serialization::DataMemberAttribute]
		System::String^ path;
		SceneResource(System::String^ resourcePath) { path = resourcePath; }
	};
}
