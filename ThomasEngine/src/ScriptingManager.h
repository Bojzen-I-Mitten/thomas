#pragma once

#include "Scene.h"
#include "object\GameObject.h"
using namespace System;
using namespace System::IO;
using namespace System::Reflection;

namespace ThomasEditor
{
	public ref class ScriptingManger
	{
	private:
		static FileSystemWatcher^ fsw;
		static String^ assemblyFolderPath;
		static Assembly^ assembly;
	public:
		static void Init() {
			fsw = gcnew FileSystemWatcher();

			assemblyFolderPath = "../Data/assembly";

			fsw->Path = assemblyFolderPath;
			fsw->Filter = "*.dll";

			fsw->Changed += gcnew FileSystemEventHandler(&ThomasEditor::ScriptingManger::OnChanged);

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
			if (File::Exists(assemblyFolderPath + "/Assembly.dll"))
			{
				if (Scene::CurrentScene)
				{
					
					Scene::SaveSceneAs(Scene::CurrentScene, tempFile);
				}
				

				array<Byte>^ bytes = File::ReadAllBytes(assemblyFolderPath + "/Assembly.dll");
				array<Byte>^ symbolBytes = File::ReadAllBytes(assemblyFolderPath + "/Assembly.pdb");
				assembly = Assembly::Load(bytes, symbolBytes);
				

				if (Scene::CurrentScene)
				{
					Scene^ oldScene = Scene::CurrentScene;

					Scene::CurrentScene = Scene::LoadScene(tempFile);
					oldScene->UnLoad();
					File::Delete(tempFile);
				}

				
			}
			fsw->EnableRaisingEvents = true;

			
		}
		static void OnChanged(System::Object ^sender, System::IO::FileSystemEventArgs ^e);
	};
}

