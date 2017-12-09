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
#include "../thomas/editor/EditorCamera.h"

namespace thomas {
	ID3D11Debug* ThomasCore::s_debug = nullptr;
	ID3D11Device* ThomasCore::s_device = nullptr;
	ID3D11DeviceContext* ThomasCore::s_context = nullptr;
	std::vector<std::string> ThomasCore::s_logOutput;
	bool ThomasCore::s_initialized;

	bool ThomasCore::Init()
	{
		if (!s_device)
		{
			LOG("DirectX Not initialized. Run initDirectX first");
			s_initialized = false;
			return false;
		}
		srand(time(NULL));


		bool init = Input::Init();

		if (init)
			init = graphics::Texture::Init();


		utils::DebugTools::Init();

		if (init)
			init = ThomasTime::Init();

		/*if (init)
			init = Sound::Init();*/


		if(init)
			init = graphics::TextRender::Initialize();

		if (init)
			init = graphics::Sprite::Initialize();

		//graphics::ParticleSystem::Init();
		if(init)
			init = graphics::Shader::Init();
		if(init)
			graphics::Material::Init();
		if (init)
			init = Physics::Init();
		if(init)
			editor::EditorCamera::Init();

		s_initialized = init;
		return s_initialized;
	}

	bool ThomasCore::InitDirectX()
	{
		if (!s_device)
		{
			bool hr = utils::D3d::CreateDeviceAndContext(s_device, s_context);
			if(!hr)
				LOG("Failed to init DirectX Device");
			#ifdef _DEBUG_DX
				utils::D3d::CreateDebug(s_debug);
			#endif // _DEBUG_DX
			return hr;
		}
		return true;
	}

	void ThomasCore::Update()
	{
		Input::Update();
		ThomasTime::Update();

		std::string title = "FPS: " + std::to_string(ThomasTime::GetFPS()) + " FrameTime: " + std::to_string(ThomasTime::GetFrameTime());

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

	bool ThomasCore::Destroy()
	{
		Scene::UnloadScene();
		Window::Destroy();
		graphics::LightManager::Destroy();
		graphics::ParticleSystem::Destroy();
		graphics::Sprite::Destroy();
		graphics::TextRender::Destroy();
		graphics::Texture::ReleaseSamplers();
		graphics::Texture::Destroy();
		graphics::Model::Destroy();
		utils::DebugTools::Destroy();
		object::Object::Destroy();
		s_context->Release();
		s_device->Release();

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
	}
	ID3D11Device * ThomasCore::GetDevice()
	{
		return s_device;
	}
	ID3D11DeviceContext* ThomasCore::GetDeviceContext()
	{
		return s_context;
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

