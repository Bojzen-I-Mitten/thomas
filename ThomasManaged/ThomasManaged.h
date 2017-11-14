// ThomasManaged.h
#pragma once
#include <Window.h>
#include <ThomasCore.h>
#include <ThomasTime.h>
#include <Input.h>
#include <Scene.h>
#pragma unmanaged
#include <TestScene.h>
#pragma managed
//#include <Sound.h>
#include <utils\DebugTools.h>
using namespace System;

namespace ThomasManaged {

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
			LOG("Thomas fully initiated, Chugga-chugga-whoo-whoo!");
		}

		static void eventHandler(IntPtr hWnd, int msg, IntPtr wParam, IntPtr lParam) {
			thomas::Window::EventHandler((HWND)hWnd.ToPointer(), msg, (WPARAM)wParam.ToPointer(), (LPARAM)lParam.ToPointer());
		}

		static void Update() 
		{
			thomas::ThomasTime::Update();
			thomas::Input::Update();
			thomas::ThomasTime::Update();
			
			
			if (thomas::Input::GetKeyDown(thomas::Input::Keys::F1))
				thomas::utils::DebugTools::ToggleVisibility();
			//thomas::Scene::UpdateCurrentScene();
			//thomas::Physics::Update();
			thomas::Scene::Render();
			//thomas::Sound::Update();

		}
	};
}
