// ThomasManaged.h

#pragma once


#pragma unmanaged

#include <thomas\ThomasCore.h>
#include <thomas\Window.h>
#include <thomas\ThomasTime.h>
#include <thomas\Input.h>
#include <thomas\utils\DebugTools.h>
#include <thomas\resource\Shader.h>
#include <thomas\graphics\Renderer.h>
#include <thomas\editor\gizmos\Gizmos.h>
#pragma managed
//#include <Sound.h>
#include "resource\Resources.h"
#include "object\GameObject.h"
#include "object\component\Transform.h"
#include "Scene.h"
#include "ScriptingManager.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Threading;
using namespace thomas;


namespace ThomasEditor {


	public ref class ThomasWrapper
	{
	private:

		static Thread^ mainThread;
		static Thread^ renderThread;
		static bool playing = false;	
		static ManualResetEvent^ RenderFinished;
		static ManualResetEvent^ UpdateFinished;
	public:

		enum class ManipulatorOperation {
			TRANSLATE,
			ROTATE,
			SCALE
		};

		static void Start() {
			thomas::ThomasCore::Init();
			if (ThomasCore::Initialized())
			{
				RenderFinished = gcnew ManualResetEvent(true);
				UpdateFinished = gcnew ManualResetEvent(false);
				ScriptingManger::Init();
				Scene::CurrentScene = gcnew Scene("test");
				LOG("Thomas fully initiated, Chugga-chugga-whoo-whoo!");
				mainThread = gcnew Thread(gcnew ThreadStart(StartEngine));
				mainThread->Name = "Thomas Engine (Main Thread)";
				mainThread->Start();

				renderThread = gcnew Thread(gcnew ThreadStart(StartRenderer));
				renderThread->Name = "Thomas Engine (Render Thread)";
				renderThread->Start();
			}

		}

		static void StartRenderer()
		{
						
			while (ThomasCore::Initialized())
			{
				UpdateFinished->WaitOne();
				UpdateFinished->Reset();
				Window::ClearAllWindows();
				thomas::graphics::Renderer::ProcessCommands();
				thomas::Window::PresentAllWindows();
				RenderFinished->Set();
			}
		}

		static void CopyCommandList()
		{
			thomas::Window::EndFrame(true);
			thomas::graphics::Renderer::TransferCommandList();
			thomas::editor::Gizmos::TransferGizmoCommands();
		}

		static void StartEngine()
		{
			while (ThomasCore::Initialized())
			{
				
				if (Scene::IsLoading())
				{
					Thread::Sleep(1000);
					continue;
				}
				
				Object^ lock = Scene::CurrentScene->GetGameObjectsLock();

				if (Window::WaitingForUpdate()) //Make sure that we are not rendering when resizing the window.
					RenderFinished->WaitOne();
				Window::Update();
				

				ThomasCore::Update();
				Monitor::Enter(lock);
					
				if (playing)
				{
					//Physics
					thomas::Physics::UpdateRigidbodies();
					for each(ThomasEditor::GameObject^ gameObject in Scene::CurrentScene->GameObjects)
					{
						if (gameObject->GetActive())
							gameObject->FixedUpdate(); //Should only be ran at fixed timeSteps.
					}
					thomas::Physics::Simulate();
				}

				//Logic
				for each(ThomasEditor::GameObject^ gameObject in Scene::CurrentScene->GameObjects)
				{
					if(gameObject->GetActive())
						gameObject->Update();
				}



				//Rendering
					
				graphics::Renderer::ClearCommands();
				editor::Gizmos::ClearGizmos();
				if (Window::GetEditorWindow() && Window::GetEditorWindow()->Initialized())
				{
						
					editor::EditorCamera::Render();
					for each(ThomasEditor::GameObject^ gameObject in Scene::CurrentScene->GameObjects)
					{
						if(gameObject->GetActive())
							gameObject->RenderGizmos();
					}

					Monitor::Enter(SelectedGameObjects);
					for each(ThomasEditor::GameObject^ gameObject in SelectedGameObjects)
					{
						if(gameObject->GetActive())
							gameObject->RenderSelectedGizmos();
					}
					Monitor::Exit(SelectedGameObjects);
					//end editor rendering

					for (object::component::Camera* camera : object::component::Camera::s_allCameras)
					{
						camera->Render();
					}
					RenderFinished->WaitOne();
					CopyCommandList();
					RenderFinished->Reset();
					UpdateFinished->Set();
				}
				Monitor::Exit(lock);
			}
			Resources::UnloadAll();
			ThomasCore::Destroy();
				
		}

		static void Exit() {
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
			Window::UpdateFocus();
			UpdateLog();
			if (thomas::editor::EditorCamera::HasSelectionChanged())
				UpdateSelectedObjects();
		}

		static void Play()
		{
			Scene::CurrentScene->Play();
			playing = true;
		}

		static bool IsPlaying()
		{
			return playing;
		}

		static void Stop()
		{
			playing = false;
			Scene::RestartCurrentScene();
		}

		static void SelectGameObject(GameObject^ gObj)
		{
			Monitor::Enter(SelectedGameObjects);
			SelectedGameObjects->Clear();
			SelectedGameObjects->Add(gObj);
			thomas::editor::EditorCamera::SelectObject((thomas::object::GameObject*)gObj->nativePtr);
			Monitor::Exit(SelectedGameObjects);

		}

		static void UnselectGameObjects()
		{
			Monitor::Enter(SelectedGameObjects);
			SelectedGameObjects->Clear();
			thomas::editor::EditorCamera::SelectObject(nullptr);
			Monitor::Exit(SelectedGameObjects);
		}

		static void UpdateSelectedObjects() {
			List<GameObject^> tempSelectedGameObjects;
			Monitor::Enter(SelectedGameObjects);
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
			Monitor::Exit(SelectedGameObjects);
			thomas::editor::EditorCamera::SetHasSelectionChanged(false);
		}

		static void SetEditorGizmoManipulatorOperation(ManipulatorOperation op)
		{
			thomas::editor::EditorCamera::SetManipulatorOperation((ImGuizmo::OPERATION)op);
		}

		static ManipulatorOperation GetEditorGizmoManipulatorOperation()
		{
			return (ManipulatorOperation)thomas::editor::EditorCamera::GetManipulatorOperation();
		}

		static void ToggleEditorGizmoManipulatorMode()
		{
			thomas::editor::EditorCamera::ToggleManipulatorMode();
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
