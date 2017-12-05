#include "ThomasCore.h"

#include "Input.h"
#include "object\Object.h"
#include "graphics\Texture.h"
#include "graphics\Renderer.h"
#include "graphics\Shader.h"
#include "graphics\Model.h"
#include "graphics\Material.h"
#include "graphics\LightManager.h"
#include "graphics\TextRender.h"
#include <assimp\Importer.hpp>
#include "graphics\Sprite.h"
#include "Scene.h"
#include "Physics.h"
#include "utils\DebugTools.h"
#include <AtlBase.h>
#include <atlconv.h>
#include "utils/d3d.h"
#include "graphics\ParticleSystem.h"
#include "Scene.h"
#include "Input.h"
#include "Window.h"
#include "ThomasTime.h"
#include <time.h>

namespace thomas {
	ID3D11Debug* ThomasCore::s_debug;
	ID3D11Device* ThomasCore::s_device;
	ID3D11DeviceContext* ThomasCore::s_context;
	IDXGISwapChain* ThomasCore::s_swapchain;
	HINSTANCE ThomasCore::s_hInstance;
	std::vector<std::string> ThomasCore::s_logOutput;
	bool ThomasCore::s_initialized;

	bool ThomasCore::Init()
	{
		srand(time(NULL));

		bool init = Window::Initialized();

		if (init)
			init = Input::Init();

		if (init)
			init = utils::D3d::Init(s_device, s_context, s_swapchain, s_debug);

		if (init)
			init = graphics::Texture::Init();

		if (init)
			init = graphics::Renderer::Init();

		utils::DebugTools::Init();

		if (init)
			init = ThomasTime::Init();

		/*if (init)
			init = Sound::Init();*/


		if(init)
			init = graphics::TextRender::Initialize();

		if (init)
			init = graphics::Sprite::Initialize();
		if (init)
			init = Physics::Init();


		utils::DebugTools::Init();

		//graphics::ParticleSystem::Init();
		
		s_initialized = init;
		return s_initialized;
	}

	HINSTANCE ThomasCore::GetHInstance()
	{
		return s_hInstance;
	}

	void ThomasCore::Update()
	{
		Input::Update();
		ThomasTime::Update();

		std::string title = "FPS: " + std::to_string(ThomasTime::GetFPS()) + " FrameTime: " + std::to_string(ThomasTime::GetFrameTime());
		SetWindowText(Window::GetWindowHandler(), CA2W(title.c_str()));

		if (Input::GetKeyDown(Input::Keys::F1))
			utils::DebugTools::ToggleVisibility();
		
		//Sound::Update();
	}

	void ThomasCore::Render()
	{
		Scene::Render();
	}

	void ThomasCore::Start()
	{


		if (s_initialized)
		{
			LOG("Thomas fully initiated, Chugga-chugga-whoo-whoo!");
			MSG msg = { 0 };
			ThomasTime::Update();

			while (WM_QUIT != msg.message)
			{
				// read messages
				if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
				{
					
					Update();
				}
			}
			Destroy();
			Window::Destroy();

		}
		else
		{
			LOG("Thomas failed to initiate :(");
			#ifdef _DEBUG
			system("pause");
			#endif // DEBUG

			
		}
			
	}
	bool ThomasCore::Initialized()
	{
		return s_initialized;
	}

	bool ThomasCore::Resize()
	{
		if (s_initialized) {
			Window::Resize();
			graphics::Renderer::Resize();
			utils::DebugTools::Resize();
			//utils::D3d::CreateDebugInfoQueue();
			return true;
		}
		return false;
		
	}

	bool ThomasCore::Destroy()
	{
		Scene::UnloadScene();
		graphics::LightManager::Destroy();
		graphics::ParticleSystem::Destroy();
		graphics::Sprite::Destroy();
		graphics::TextRender::Destroy();
		graphics::Texture::ReleaseSamplers();
		graphics::Texture::Destroy();
		graphics::Model::Destroy();
		graphics::Renderer::Destroy();
		utils::DebugTools::Destroy();
		object::Object::Destroy();
		s_swapchain->Release();
		s_context->Release();
		s_device->Release();

		
		s_swapchain = nullptr;
		s_context = nullptr;
		s_device = nullptr;
		

		#ifdef _DEBUG
		s_debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		s_debug->Release();
		s_debug = nullptr;
		#endif // _DEBUG

		//Sound::Destroy();

		return true;
	}
	void ThomasCore::Exit()
	{
		Window::Destroy();
	}
	ID3D11Device * ThomasCore::GetDevice()
	{
		return s_device;
	}
	ID3D11DeviceContext* ThomasCore::GetDeviceContext()
	{
		return s_context;
	}
	IDXGISwapChain * ThomasCore::GetSwapChain()
	{
		return s_swapchain;
	}
	void ThomasCore::LogOutput(std::string message)
	{

		s_logOutput.push_back(message);
	}

	std::vector<std::string>* ThomasCore::GetLogOutput()
	{
		return &s_logOutput;
	}
	void ThomasCore::ClearLogOutput()
	{
		s_logOutput.clear();
	}
}

