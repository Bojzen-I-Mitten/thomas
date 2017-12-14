// ThomasManaged.h
#pragma once




#pragma unmanaged
#include <thomas\ThomasCore.h>
#include <thomas\Scene.h>
#include <thomas\Window.h>
#include <thomas\ThomasTime.h>
#include <thomas\Input.h>
#include <thomas\utils\DebugTools.h>
#include <thomas\graphics\Shader.h>

#pragma managed
//#include <Sound.h>


#include "Tests\TestScene.h"
#include "object\GameObject.h"
#include "object\component\Transform.h"


using namespace System;
using namespace System::Collections::Generic;
using namespace System::Threading;
using namespace thomas;


namespace ThomasEditor {

	public ref class ThomasWrapper
	{
	private:

		static Thread^ testThread;


	public:

		static void Start() {
			thomas::ThomasCore::Init();
			if (ThomasCore::Initialized())
			{
				thomas::Scene::LoadScene<TestScene>();

				LOG("Thomas fully initiated, Chugga-chugga-whoo-whoo!");
				testThread = gcnew Thread(gcnew ThreadStart(StartEngine));
				testThread->Name = "Thomas Engine";
				testThread->Start();
				
			}

		}

		static void StartEngine()
		{
			while (ThomasCore::Initialized())
			{
				{
					
					ThomasCore::Update();
					for each(ThomasEditor::GameObject^ gameObject in ThomasEditor::GameObject::GameObjects)
					{
						gameObject->UpdateComponents();
					}
					ThomasCore::Render();
				}
			}
			ThomasCore::Destroy();
				
		}

		static void Stop() {
			thomas::ThomasCore::Exit();
		}
		static ObservableCollection<GameObject^>^ SelectedGameObjects = gcnew ObservableCollection<GameObject^>();
		static ObservableCollection<String^>^ OutputLog = gcnew ObservableCollection<String^>();

		static void CreateThomasWindow(IntPtr hWnd, bool isEditor)
		{
			if (thomas::ThomasCore::Initialized()) {
				if (isEditor)
					thomas::Window::InitEditor((HWND)hWnd.ToPointer());
				else
					thomas::Window::Create((HWND)hWnd.ToPointer());

			}
				
		}

		static property float FrameRate
		{
			float get() { return thomas::ThomasTime::GetFPS(); }
		}
		

		static void eventHandler(IntPtr hWnd, int msg, IntPtr wParam, IntPtr lParam) {
			thomas::Window::EventHandler((HWND)hWnd.ToPointer(), msg, (WPARAM)wParam.ToPointer(), (LPARAM)lParam.ToPointer());
		}

		static void Resize(IntPtr hWnd, double width, double height)
		{
			Window* window = thomas::Window::GetWindow((HWND)hWnd.ToPointer());
			if (window)
				window->QueueResize();
		}

		static float timeSinceLastUpdate = 100000;
		static void Update() 
		{
			/*if (thomas::ThomasCore::Initialized() && thomas::Scene::GetCurrentScene() != NULL)
			{
				timeSinceLastUpdate += thomas::ThomasTime::GetActualDeltaTime();
				if (timeSinceLastUpdate > 1.0f / 120.0f)
				{
					thomas::Scene::ClearRenderQueue();
					thomas::ThomasCore::Update();
					thomas::Scene::UpdateCurrentScene();
					for each(ThomasEditor::GameObject^ gameObject in ThomasEditor::GameObject::GameObjects)
					{
						gameObject->UpdateComponents();
					}
					thomas::Physics::Update();

					UpdateLog();
					timeSinceLastUpdate = 0.0f;
				}
				thomas::ThomasCore::Render();
				if (thomas::editor::EditorCamera::HasSelectionChanged())
					UpdateSelectedObjects();
				
			}*/
			Window::UpdateFocus();
			UpdateLog();
			if (thomas::editor::EditorCamera::HasSelectionChanged())
				UpdateSelectedObjects();
		}

		static void SelectGameObject(GameObject^ gObj)
		{
			SelectedGameObjects->Add(gObj);
			thomas::editor::EditorCamera::SelectObject((thomas::object::GameObject*)gObj->nativePtr);
		}
		static void UpdateSelectedObjects() {
			List<GameObject^> tempSelectedGameObjects;
			for (thomas::object::GameObject* gameObject : thomas::editor::EditorCamera::GetSelectedObjects())
			{
				GameObject^ gObj = (GameObject^)ThomasEditor::Object::GetObject(gameObject);
				if (gObj)
					tempSelectedGameObjects.Add(gObj);
			}
			if (tempSelectedGameObjects.Count == SelectedGameObjects->Count)
			{
				if(tempSelectedGameObjects.Count > 0)
					if (tempSelectedGameObjects[0] != SelectedGameObjects[0])
					{
						SelectedGameObjects->Clear();
						for each(GameObject^ gObj in tempSelectedGameObjects)
							SelectedGameObjects->Add(gObj);
					}
			}
			else
			{
				SelectedGameObjects->Clear();
				for each(GameObject^ gObj in tempSelectedGameObjects)
					SelectedGameObjects->Add(gObj);
			}

			thomas::editor::EditorCamera::SetHasSelectionChanged(false);
		}

		static void UpdateLog() {
			std::vector<std::string> nativeOutputs = thomas::ThomasCore::GetLogOutput();
			
			for (int i = 0; i < nativeOutputs.size(); i++) {
				String^ output = gcnew String(nativeOutputs.at(i).c_str());
				if (OutputLog->Count == 0 || OutputLog[OutputLog->Count - 1] != output)
				{
					OutputLog->Add(output);
					if (OutputLog->Count > 10)
						OutputLog->RemoveAt(0);
				}
			}
			thomas::ThomasCore::ClearLogOutput();
		}
	};
}
