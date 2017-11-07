#pragma once
#include <d3d11.h>
#include "Common.h"


namespace thomas
{
	class THOMAS_API ThomasCore
	{
	private:
		static void Update();
	public:
		static bool Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, LONG windowWidth, LONG windowHeight, LPWSTR title);
		
		static HINSTANCE GetHInstance();

		static void Start();
		static bool Initialized();
		static bool Destroy();
		static void Exit();

		static ID3D11Device* GetDevice();
		static ID3D11DeviceContext* GetDeviceContext();
		static IDXGISwapChain* GetSwapChain();

	private:
		static HINSTANCE s_hInstance;
		static bool s_initialized;
		static ID3D11Device* s_device;
		static ID3D11DeviceContext* s_context;
		static IDXGISwapChain* s_swapchain;
		static ID3D11Debug* s_debug;

	};
}