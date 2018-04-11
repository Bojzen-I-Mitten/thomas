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

			static ID3D11Buffer* CreateStagingBuffer(UINT byte_width, UINT byte_stride);

			template<typename T>
			static ID3D11Buffer* CreateBufferFromStruct(T& dataStruct, D3D11_BIND_FLAG bindFlag);

			template<typename T>
			static ID3D11Buffer* CreateDynamicBufferFromStruct(T& dataStruct, D3D11_BIND_FLAG bindFlag);

			template <typename T>
			static ID3D11Buffer* CreateBufferFromVector(const std::vector<T>& vectorData, D3D11_BIND_FLAG bindFlag);

			template <typename T>
			static ID3D11Buffer* CreateDynamicBufferFromVector(const std::vector<T>& vectorData, D3D11_BIND_FLAG bindFlag);

			template<typename T>
			static bool FillBuffer(ID3D11Buffer* buffer, T& data);

			template<typename T>
			static bool FillDynamicBufferStruct(ID3D11Buffer* buffer, T& data);

			template<typename T>
			static bool FillDynamicBufferVector(ID3D11Buffer* buffer, const std::vector<T>& vectorData);

		private:

		};
		template<typename T>
		ID3D11Buffer* D3d::CreateBufferFromStruct(T& dataStruct, D3D11_BIND_FLAG bindFlag)
		{
			ID3D11Buffer* buffer;
			D3D11_BUFFER_DESC bufferDesc;
			bufferDesc.ByteWidth = sizeof(dataStruct);
			bufferDesc.Usage = D3D11_USAGE_DEFAULT; //TODO: Maybe dynamic for map/unmap
			bufferDesc.BindFlags = bindFlag;
			bufferDesc.CPUAccessFlags = 0; //CPU if dynamic
			bufferDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = &dataStruct;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;

			HRESULT result = ThomasCore::GetDevice()->CreateBuffer(&bufferDesc, &InitData, &buffer);

			if (result != S_OK)
				LOG(result);

			if (result == S_OK)
				return buffer;

			return NULL;

		}

		template<typename T>
		ID3D11Buffer* D3d::CreateDynamicBufferFromStruct(T& dataStruct, D3D11_BIND_FLAG bindFlag)
		{
			ID3D11Buffer* buffer;
			D3D11_BUFFER_DESC bufferDesc;
			bufferDesc.ByteWidth = sizeof(dataStruct);
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC; //TODO: Maybe dynamic for map/unmap
			bufferDesc.BindFlags = bindFlag;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = &dataStruct;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;

			HRESULT result = ThomasCore::GetDevice()->CreateBuffer(&bufferDesc, &InitData, &buffer);

			if (result != S_OK)
				LOG(result);

			if (result == S_OK)
				return buffer;

			return NULL;

		}

		template<typename T>
		bool D3d::FillBuffer(ID3D11Buffer* buffer, T& data)
		{
			ThomasCore::GetDeviceContext()->UpdateSubresource(buffer, 0, NULL, &data, 0, 0);
			return true;
		}

		template<typename T>
		bool D3d::FillDynamicBufferVector(ID3D11Buffer * buffer, const std::vector<T>& vectorData)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
			ThomasCore::GetDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			memcpy(mappedResource.pData, &vectorData[0], sizeof(vectorData[0])*vectorData.size());
			ThomasCore::GetDeviceContext()->Unmap(buffer, 0);
			return true;
		}

		template<typename T>
		bool D3d::FillDynamicBufferStruct(ID3D11Buffer * buffer, T& data)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
			ThomasCore::GetDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			memcpy(mappedResource.pData, &data, sizeof(data));
			ThomasCore::GetDeviceContext()->Unmap(buffer, 0);
			return true;
		}

		template <typename T>
		ID3D11Buffer* D3d::CreateBufferFromVector(const std::vector<T>& vectorData, D3D11_BIND_FLAG bindFlag)
		{
			ID3D11Buffer* buffer;
			D3D11_BUFFER_DESC bufferDesc;
			bufferDesc.ByteWidth = sizeof(vectorData[0]) * vectorData.size();
			bufferDesc.Usage = D3D11_USAGE_DEFAULT; //TODO: Maybe dynamic for map/unmap
			bufferDesc.BindFlags = bindFlag;
			bufferDesc.CPUAccessFlags = 0; //CPU if dynamic
			bufferDesc.MiscFlags = 0;


			const T* data = &vectorData[0];

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = data;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;

			HRESULT result = ThomasCore::GetDevice()->CreateBuffer(&bufferDesc, &InitData, &buffer);

			if (result != S_OK)
				LOG(result);

			if (result == S_OK)
				return buffer;

			return NULL;

		}

		template <typename T>
		ID3D11Buffer* D3d::CreateDynamicBufferFromVector(const std::vector<T>& vectorData, D3D11_BIND_FLAG bindFlag)
		{
			ID3D11Buffer* buffer;
			D3D11_BUFFER_DESC bufferDesc;
			bufferDesc.ByteWidth = sizeof(vectorData[0]) * vectorData.size();
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC; //TODO: Maybe dynamic for map/unmap
			bufferDesc.BindFlags = bindFlag;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //CPU if dynamic
			bufferDesc.MiscFlags = 0;


			const T* data = &vectorData[0];

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = data;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;

			HRESULT result = ThomasCore::GetDevice()->CreateBuffer(&bufferDesc, &InitData, &buffer);

			if (result != S_OK)
				LOG(result);

			if (result == S_OK)
				return buffer;

			return NULL;

		}

		

	}

}