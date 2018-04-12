#pragma once
#include <d3dcompiler.h>
#include <d3d11.h>
#include <string>
#include <vector>
#include "../ThomasCore.h"
#include "../Common.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment(lib, "Dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace thomas 
{
	class Window;
	namespace utils
	{	
		class D3d
		{
		public:
						
			static bool CreateBackBuffer(ID3D11Device* device, IDXGISwapChain* swapchain, ID3D11RenderTargetView*& backBuffer, ID3D11ShaderResourceView*& backBufferSRV);
			static bool CreateDepthStencilView(ID3D11Device* device, LONG width, LONG height, ID3D11DepthStencilView *& stencilView, ID3D11DepthStencilView*& depthStencilViewReadOnly, ID3D11ShaderResourceView *& depthBufferSRV);

			static bool CreateDeviceAndContext(ID3D11Device*& device, ID3D11DeviceContext*& context);
			static bool CreateSwapChain(LONG width, LONG height, HWND handle, IDXGISwapChain*& swapchain, ID3D11Device* device);
			static bool CreateDebug(ID3D11Debug*& debug);

			static ID3D11InfoQueue* CreateDebugInfoQueue();

			static bool CreateRenderTargetView(LONG width, LONG height, ID3D11RenderTargetView*& rtv, ID3D11ShaderResourceView*& srv);

			static ID3D11DepthStencilState* CreateDepthStencilState(D3D11_COMPARISON_FUNC func, bool depth);
			
			static bool LoadTextureFromFile(ID3D11Device* device, _In_opt_ ID3D11DeviceContext* context, std::string fileName, _Outptr_opt_ ID3D11Resource*& texture, _Outptr_opt_ ID3D11ShaderResourceView*& textureView);
			static bool LoadCubeTextureFromFile(ID3D11Device* device, _In_opt_ ID3D11DeviceContext* context, std::string fileName, _Outptr_opt_ ID3D11Resource*& texture, _Outptr_opt_ ID3D11ShaderResourceView*& textureView);
			static ID3D11RasterizerState* CreateRasterizer(D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode);


			static ID3D11ShaderResourceView* CreateFresnel(int fresnelTexSize, float blending);

			static void CreateTextureAndViews(UINT width, UINT height, DXGI_FORMAT format, ID3D11Texture2D*& tex, ID3D11ShaderResourceView*& SRV, ID3D11RenderTargetView*& RTV);

			static void CreateBufferAndUAV(void* data, UINT byte_width, UINT byte_stride, ID3D11Buffer*& buffer, ID3D11UnorderedAccessView*& UAV, ID3D11ShaderResourceView*& SRV);
			
			static void CreateCPUReadBufferAndUAV(void* data, UINT byte_width, UINT byte_stride, ID3D11Buffer*& buffer, ID3D11UnorderedAccessView*& UAV);

			static ID3D11Buffer * CreateStagingBuffer(UINT byte_width, UINT byte_stride);
		private:

		};
			

	}

}