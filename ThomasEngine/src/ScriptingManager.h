#pragma once

#include "Scene.h"
#include "object\GameObject.h"
#include "Application.h"
using namespace System;
using namespace System::IO;
using namespace System::Reflection;

namespace ThomasEngine
{
	public ref class ScriptingManger
	{
	private:
		static FileSystemWatcher^ fsw;
		static Assembly^ assembly;
	public:
		static void Init() {
			fsw = gcnew FileSystemWatcher();
			fsw->Filter = "Assembly.dll";
			fsw->Changed += gcnew FileSystemEventHandler(&ScriptingManger::OnChanged);
			Application::currentProjectChanged += gcnew Application::CurrentProjectChangedEvent(&ScriptingManger::OnCurrentProjectChanged);
		}

		static void OnCurrentProjectChanged(Project^ newProject) {
			String^ assemblyFolderPath = newProject->assembly;
#ifdef _DEBUG
			assemblyFolderPath += "/Debug";
#else
			assemblyFolderPath += "/Release";
#endif
			fsw->EnableRaisingEvents = false;
			fsw->Path = assemblyFolderPath;
			fsw->EnableRaisingEvents = true;
			LoadAssembly();
		}
		static Assembly^ GetAssembly()
		{
			return assembly;
		}

		delegate void OnChanged(Object^ sender, FileSystemEventArgs e);
		

		static bool IsFileLocked(FileInfo^ file)
		{
			FileStream^ stream;
			try {
				stream = file->Open(FileMode::Open, FileAccess::ReadWrite, FileShare::None);
			}
			catch (IOException^) {
				return true;
			}
			finally{
				if (stream)
					stream->Close();
			}
			return false;
		}

		static void LoadAssembly() {

			String^ tempFile = Path::Combine(Environment::GetFolderPath(Environment::SpecialFolder::LocalApplicationData), "thomas/scene.tds");
			if (File::Exists(fsw->Path + "/Assembly.dll"))
			{
				String^ currentSavePath;
				if (Scene::CurrentScene)
				{
					currentSavePath = Scene::CurrentScene->RelativeSavePath;
					Scene::SaveSceneAs(Scene::CurrentScene, tempFile);
				}
				

				array<Byte>^ bytes = File::ReadAllBytes(fsw->Path + "/Assembly.dll");
				array<Byte>^ symbolBytes = File::ReadAllBytes(fsw->Path + "/Assembly.pdb");
				assembly = Assembly::Load(bytes, symbolBytes);
				

				if (Scene::CurrentScene)
				{
					Scene^ oldScene = Scene::CurrentScene;

					Scene::CurrentScene = Scene::LoadScene(tempFile);
					oldScene->UnLoad();
					File::Delete(tempFile);
					Scene::CurrentScene->RelativeSavePath = currentSavePath;
					if(Application::currentProject)
						Application::currentProject->currentScenePath = currentSavePath;
				}

				
			}
			fsw->EnableRaisingEvents = true;

			
		}
		static void OnChanged(System::Object ^sender, System::IO::FileSystemEventArgs ^e);
	};
}