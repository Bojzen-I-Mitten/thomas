#pragma once
#include <d3d11.h>

#include <vector>
#include <string>

namespace thomas
{
	class ThomasCore
	{
	public:
		
		static bool Init();

		static HINSTANCE GetHInstance();

		static void Start();
		static bool Initialized();
		static bool Resize();
		static bool Destroy();
		static void Exit();

		static void Update();
		static void Render();

		static ID3D11Device* GetDevice();
		static ID3D11DeviceContext* GetDeviceContext();
		static IDXGISwapChain* GetSwapChain();

		static void LogOutput(std::string message);
		static std::vector<std::string>* GetLogOutput();
		static void ClearLogOutput();

	private:
		static HINSTANCE s_hInstance;
		static bool s_initialized;
		static ID3D11Device* s_device;
		static ID3D11DeviceContext* s_context;
		static IDXGISwapChain* s_swapchain;
		static ID3D11Debug* s_debug;
		static std::vector<std::string> s_logOutput;

	};
}