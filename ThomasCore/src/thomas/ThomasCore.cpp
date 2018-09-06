#include "ThomasCore.h"
#include "Sound.h"
#include "Input.h"
#include "Window.h"
#include "ThomasTime.h"
#include "object\Object.h"
#include "resource\texture\Texture2D.h"
#include "resource\Shader.h"
#include "resource\Material.h"
#include "editor\EditorCamera.h"
#include "editor\gizmos\Gizmos.h"
#include "utils\Primitives.h"
#include <D3d11_4.h>

namespace thomas 
{
	ID3D11Debug* ThomasCore::s_debug;
	ID3D11Device* ThomasCore::s_device;
	ID3D11DeviceContext* ThomasCore::s_context;
	std::vector<std::string> ThomasCore::s_logOutput;
	bool ThomasCore::s_clearLog;
	bool ThomasCore::s_initialized;
	ImGuiContext* ThomasCore::s_imGuiContext;

	bool ThomasCore::Init()
	{
		s_imGuiContext = ImGui::CreateContext();
		s_logOutput.reserve(10);

		//Init all required classes
		assert(InitDirectX());
		assert(Input::Init());
		resource::Texture2D::Init();
		assert(ThomasTime::Init());
		assert(Sound::Init());
		assert(resource::Shader::Init());

		resource::Material::Init();
		assert(Physics::Init());
		editor::EditorCamera::Init();
		editor::Gizmos::Init();
		utils::Primitives::Init();

		s_initialized = true;
		return s_initialized;
	}

	void ThomasCore::Update()
	{
		if (s_clearLog)
		{
			s_logOutput.clear();
			s_clearLog = false;
		}

		object::Object::Clean();
		editor::EditorCamera::Update();
		resource::Shader::Update();
		Input::Update();		
		Sound::Update();
	}

	void ThomasCore::Exit()
	{
		s_initialized = false;
	}

	bool ThomasCore::Initialized()
	{
		return s_initialized;
	}

	bool ThomasCore::Destroy()
	{	
		s_context->ClearState();
		s_context->Flush();

		//Destroy all objects
		Window::Destroy();
		//graphics::LightManager::Destroy();
		resource::Shader::DestroyAllShaders();
		resource::Material::Destroy();
		resource::Texture2D::Destroy();
		object::Object::Destroy();
		editor::EditorCamera::Destroy();
		editor::Gizmos::Destroy();
		utils::Primitives::Destroy();
		Physics::Destroy();
		Sound::Destroy();
		ImGui::DestroyContext(s_imGuiContext);

		//Release
		s_context->ClearState();
		s_context->Flush();
		SAFE_RELEASE(s_context);
		SAFE_RELEASE(s_device);

		#ifdef _DEBUG_DX
			s_debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
			SAFE_RELEASE(s_debug);
		#endif

		return true;
	}

	ID3D11Device * ThomasCore::GetDevice()
	{
		return s_device;
	}

	ID3D11DeviceContext* ThomasCore::GetDeviceContext()
	{
		return s_context;
	}

	std::vector<std::string> ThomasCore::GetLogOutput()
	{
		return s_logOutput;
	}

	void ThomasCore::LogOutput(const std::string & message)
	{
		s_logOutput.push_back(message);
		if (s_logOutput.size() > 10)
			s_logOutput.erase(s_logOutput.begin());
	}

	void ThomasCore::ClearLogOutput()
	{
		s_clearLog = true;
	}

	bool ThomasCore::InitDirectX()
	{
		if (!s_device)
		{
			CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
			assert(CreateDeviceAndContext());
#ifdef _DEBUG_DX
			assert(!ThomasCore::GetDevice()->QueryInterface(IID_PPV_ARGS(&s_debug)));
#endif
			return true;
		}
		return true;
	}

	bool ThomasCore::CreateDeviceAndContext()
	{
		HRESULT hr = D3D11CreateDevice(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
#ifdef _DEBUG_DX
			D3D11_CREATE_DEVICE_DEBUG,
#else
			NULL,
#endif
			NULL,
			NULL,
			D3D11_SDK_VERSION,
			&s_device,
			NULL,
			&s_context
		);

		if (FAILED(hr))
		{
			LOG(hr);
			return false;
		}

		ID3D11Multithread* multi;
		hr = s_device->QueryInterface(IID_PPV_ARGS(&multi));
		if (SUCCEEDED(hr) && multi != NULL)
		{
			multi->SetMultithreadProtected(TRUE);
			SAFE_RELEASE(multi);
		}
		else
			LOG_HR(hr);
		return true;
	}
}

