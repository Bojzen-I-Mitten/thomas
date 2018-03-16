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
		static String^ assemblyPath;
		static String^ symbolPath;
		static Assembly^ assembly;
	public:
		static void Init() {
			fsw = gcnew FileSystemWatcher();

		}

		static Assembly^ GetAssembly()
		{
			return assembly;
		}

		static void LoadAssembly() {

			String^ tempFile = Path::Combine(Environment::GetFolderPath(Environment::SpecialFolder::LocalApplicationData), "thomas/scene.tds");


			Scene::SaveScene(Scene::CurrentScene, tempFile);

			array<Byte>^ bytes = File::ReadAllBytes(assemblyPath);
			array<Byte>^ symbolBytes = File::ReadAllBytes(symbolPath);
			assembly = Assembly::Load(bytes, symbolBytes);

			Scene::CurrentScene->UnLoad();

			Scene::CurrentScene = Scene::LoadScene(tempFile);

			File::Delete(tempFile);
			
		}
	};
}

