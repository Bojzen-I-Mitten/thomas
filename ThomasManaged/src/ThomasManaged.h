// ThomasManaged.h
#pragma once

#include <thomas\Window.h>
#include <thomas\ThomasCore.h>
#include <thomas\ThomasTime.h>
#include <thomas\Input.h>


#include <thomas\Scene.h>
#pragma unmanaged
#include "Tests\TestScene.h"
#pragma managed
//#include <Sound.h>
#include <thomas\utils\DebugTools.h>

#include <thomas\graphics\Shader.h>
#include "object\GameObject.h"
#include "object\component\Transform.h"
using namespace System;
using namespace System::Collections::Generic;
using namespace thomas;


namespace ThomasEditor {

	public ref class ThomasWrapper
	{
	private:
		static void Start() {
			thomas::ThomasCore::Init();
			thomas::Scene::LoadScene<TestScene>();
			if(thomas::Window::GetEditorWindow())
				SetFocus(thomas::Window::GetEditorWindow()->GetWindowHandler());
			LOG("Thomas fully initiated, Chugga-chugga-whoo-whoo!");
		}
	public:
		static void CreateThomasWindow(IntPtr hWnd, bool isEditor)
		{
			if (thomas::ThomasCore::InitDirectX()) {
				if (isEditor)
					thomas::Window::InitEditor((HWND)hWnd.ToPointer());
				else
					thomas::Window::Create((HWND)hWnd.ToPointer());

				if (!ThomasCore::Initialized())
					Start();
			}
				
		}

		

		static void eventHandler(IntPtr hWnd, int msg, IntPtr wParam, IntPtr lParam) {
			thomas::Window::EventHandler((HWND)hWnd.ToPointer(), msg, (WPARAM)wParam.ToPointer(), (LPARAM)lParam.ToPointer());
		}

		static void Resize(IntPtr hWnd)
		{
			Window* window = thomas::Window::GetWindow((HWND)hWnd.ToPointer());
			if (window)
				window->Resize();
		}

		static void Update() 
		{

			if (thomas::ThomasCore::Initialized() && thomas::Scene::GetCurrentScene() != NULL)
			{
				thomas::ThomasCore::Update();
				thomas::Scene::UpdateCurrentScene();
				for each(ThomasEditor::GameObject^ gameObject in ThomasEditor::GameObject::GameObjects)
				{
					gameObject->UpdateComponents();
				}
				thomas::Physics::Update();
				thomas::ThomasCore::Render();
					
			}
			
		}

		static List<String^>^ GetLogOutput() {
			List<String^>^ outputs = gcnew List<String^>();
			std::vector<std::string>* nativeOutputs = thomas::ThomasCore::GetLogOutput();
			for (int i = 0; i < nativeOutputs->size(); i++) {
				outputs->Add(gcnew String(nativeOutputs->at(i).c_str()));
			}
			thomas::ThomasCore::ClearLogOutput();
			return outputs;
		}
	};
}
