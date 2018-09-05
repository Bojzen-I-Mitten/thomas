// ThomasManaged.h

#pragma once


#pragma unmanaged

#include <thomas\ThomasCore.h>
#include <thomas\Window.h>
#include <thomas\ThomasTime.h>
#include <thomas\Input.h>
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


namespace ThomasEngine {


	public ref class ThomasWrapper
	{
	private:

		static bool updateEditor = false;

		static Thread^ mainThread;
		static Thread^ renderThread;
		static bool playing = false;	
		static ManualResetEvent^ RenderFinished;
		static ManualResetEvent^ UpdateFinished;
	public:
		delegate void UpdateEditor();
		static event UpdateEditor^ OnEditorUpdate;

		enum class ManipulatorOperation {
			TRANSLATE,
			ROTATE,
			SCALE
		};

		static void Start() {
			thomas::ThomasCore::Init();
			if (ThomasCore::Initialized())
			{
				Resources::LoadAll(Application::editorAssets);
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

		static void UpdateEditor()
		{
			updateEditor = true;
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
				thomas::ThomasTime::Update();
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
					for (int i = 0; i < Scene::CurrentScene->GameObjects->Count; i++)
					{
						GameObject^ gameObject = Scene::CurrentScene->GameObjects[i];
						if (gameObject->GetActive())
							gameObject->FixedUpdate(); //Should only be ran at fixed timeSteps.
					}
					thomas::Physics::Simulate();
				}

				//Logic
				for (int i = 0; i < Scene::CurrentScene->GameObjects->Count; i++)
				{
					GameObject^ gameObject = Scene::CurrentScene->GameObjects[i];
					if(gameObject->GetActive())
						gameObject->Update();
				}



				//Rendering
					
				graphics::Renderer::ClearCommands();
				editor::Gizmos::ClearGizmos();
				if (Window::GetEditorWindow() && Window::GetEditorWindow()->Initialized())
				{
						
					editor::EditorCamera::Render();
					for (int i = 0; i < Scene::CurrentScene->GameObjects->Count; i++)
					{
						GameObject^ gameObject = Scene::CurrentScene->GameObjects[i];
						if(gameObject->GetActive())
							gameObject->RenderGizmos();
					}

					Monitor::Enter(SelectedGameObjects);
					for each(ThomasEngine::GameObject^ gameObject in SelectedGameObjects)
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

				if (updateEditor)
					OnEditorUpdate();
				updateEditor = false;
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

		static void Update() 
		{
			Window::UpdateFocus();
			UpdateLog();
			if (thomas::editor::EditorCamera::HasSelectionChanged())
				UpdateSelectedObjects();
		}

		static Guid selectedGUID;
		static void Play()
		{
			ThomasEngine::Resources::OnPlay();
			Scene::CurrentScene->Play();
			playing = true;
			
		}

		static bool IsPlaying()
		{
			return playing;
		}

		static void Stop()
		{
			if (SelectedGameObjects->Count > 0)
				selectedGUID = SelectedGameObjects[0]->m_guid;
			else
				selectedGUID = Guid::Empty;
			playing = false;
			Scene::RestartCurrentScene();
			ThomasEngine::Resources::OnStop();
			if (selectedGUID != Guid::Empty)
			{
				GameObject^ gObj = (GameObject^)ThomasEngine::Object::Find(selectedGUID);
				if (gObj)
					SelectGameObject(gObj);
			}
				
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
				GameObject^ gObj = (GameObject^)ThomasEngine::Object::GetObject(gameObject);
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
