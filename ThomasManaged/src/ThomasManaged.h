// ThomasManaged.h
#pragma once

#include <thomas\Window.h>
#include <thomas\ThomasCore.h>
#include <thomas\ThomasTime.h>
#include <thomas\Input.h>


#include <thomas\Scene.h>
#pragma unmanaged
#include <thomas\TestScene.h>
#pragma managed
//#include <Sound.h>
#include <thomas\utils\DebugTools.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace thomas;
using namespace thomas::object;



namespace ThomasEditor {

	public ref class ThomasWrapper
	{

	public:
		static bool Init(IntPtr hWnd) {
			thomas::Window::Init((HWND)hWnd.ToPointer());
			thomas::ThomasCore::Init();
			thomas::Scene::LoadScene<TestScene>();
			return thomas::ThomasCore::Initialized();
		}
		static void Start() {
			SetFocus(thomas::Window::GetWindowHandler());
			LOG("Thomas fully initiated, Chugga-chugga-whoo-whoo!");
		}

		static void eventHandler(IntPtr hWnd, int msg, IntPtr wParam, IntPtr lParam) {
			thomas::Window::EventHandler((HWND)hWnd.ToPointer(), msg, (WPARAM)wParam.ToPointer(), (LPARAM)lParam.ToPointer());
		}

		static GameObject& Test() {
			return *GameObject::GetAllGameObjectsInScene(thomas::Scene::GetCurrentScene())[0];
		}

		static void Resize()
		{
			thomas::ThomasCore::Resize();
		}

		static List<String^>^ GetObjects() {
			List<String^>^ objects = gcnew List<String^>();
			auto gameObjects = thomas::object::GameObject::GetAllGameObjectsInScene(thomas::Scene::GetCurrentScene());
			for (int i = 0; i < gameObjects.size(); i++) {
				objects->Add(gcnew String(gameObjects[i]->GetType().c_str()));
			}
			return objects;
		}

		static void Update() 
		{

			if (thomas::ThomasCore::Initialized() && thomas::Scene::GetCurrentScene() != NULL)
			{
				thomas::ThomasCore::Update();
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
