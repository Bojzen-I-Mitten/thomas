#pragma once
#include "D3D.h"
#include "..\Common.h"
#include "..\Window.h"
#include "DirectXTK/WICTextureLoader.h"
#include "DirectXTK/DDSTextureLoader.h"
#include <dxgi.h>
#include <D3d11_4.h>
#include <AtlBase.h>
#include <atlconv.h>

namespace thomas
{
	namespace utils
	{
		bool D3d::CreateRenderTargetView(LONG width, LONG height, ID3D11RenderTargetView *& rtv, ID3D11ShaderResourceView *& srv)
		{
			D3D11_TEXTURE2D_DESC textureDesc;
			ZeroMemory(&textureDesc, sizeof(textureDesc));
			textureDesc.Width = width;
			textureDesc.Height = height;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = 0;//D3D11_RESOURCE_MISC_GENERATE_MIPS;

			D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
			ZeroMemory(&viewDesc, sizeof(viewDesc));
			viewDesc.Format = textureDesc.Format;
			viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			viewDesc.Texture2D.MipLevels = 1;
			viewDesc.Texture2D.MostDetailedMip = 0;

			ID3D11Texture2D* quadTexture;

			ThomasCore::GetDevice()->CreateTexture2D(&textureDesc, NULL, &quadTexture);
			ThomasCore::GetDevice()->CreateShaderResourceView(quadTexture, &viewDesc, &srv);
			ThomasCore::GetDevice()->CreateRenderTargetView(quadTexture, NULL, &rtv);
			quadTexture->Release();
			
			return true;
		}

		bool D3d::CreateTexture(void* initData, int width, int height, DXGI_FORMAT format, ID3D11Texture2D *& tex, ID3D11ShaderResourceView *& SRV, bool mipMaps, int mipLevels=1)
		{
			D3D11_TEXTURE2D_DESC textureDesc;
			ZeroMemory(&textureDesc, sizeof(textureDesc));
			textureDesc.Width = width;
			textureDesc.Height = height;
			textureDesc.MipLevels = mipLevels;
			textureDesc.ArraySize = 1;
			textureDesc.Format = format;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = mipMaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

			D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
			ZeroMemory(&viewDesc, sizeof(viewDesc));
			viewDesc.Format = textureDesc.Format;
			viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			viewDesc.Texture2D.MipLevels = mipLevels;
			viewDesc.Texture2D.MostDetailedMip = 0;
			
			HRESULT hr;
			if (initData)
			{
				D3D11_SUBRESOURCE_DATA TexInitData;
				ZeroMemory(&TexInitData, sizeof(D3D11_SUBRESOURCE_DATA));
				TexInitData.pSysMem = initData;
				TexInitData.SysMemPitch = static_cast<UINT>(4 * width);
				TexInitData.SysMemSlicePitch = static_cast<UINT>(4 * width * height);
				hr = ThomasCore::GetDevice()->CreateTexture2D(&textureDesc, &TexInitData, &tex);
			}
			else
			{
				hr = ThomasCore::GetDevice()->CreateTexture2D(&textureDesc, NULL, &tex);
			}
			if(hr == S_OK)
				hr = ThomasCore::GetDevice()->CreateShaderResourceView(tex, &viewDesc, &SRV);
			return hr == S_OK;
		}
	
		bool D3d::CreateSwapChain(LONG width, LONG height, HWND handle, IDXGISwapChain*& swapchain, ID3D11Device* device)
		{
			IDXGIDevice * dxgiDevice = 0;
			HRESULT hr = ThomasCore::GetDevice()->QueryInterface(__uuidof(IDXGIDevice), (void **)& dxgiDevice);
			if (SUCCEEDED(hr))
			{
				IDXGIAdapter * dxgiAdapter = 0;
				hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void **)& dxgiAdapter);
				if (SUCCEEDED(hr))
				{
					IDXGIFactory * dxgiFactory = 0;
					hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void **)& dxgiFactory);
					if (SUCCEEDED(hr))
					{
						DXGI_SWAP_CHAIN_DESC scd;
						ZeroMemory(&scd, sizeof(scd));

						scd.BufferCount = 1;
						scd.BufferDesc.Height = height;
						scd.BufferDesc.Width = width;
						scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
						scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
						scd.OutputWindow = handle;
						scd.Flags = 0;

						scd.SampleDesc.Count = THOMAS_AA_COUNT; //Make this costomizable!!!
						scd.SampleDesc.Quality = THOMAS_AA_QUALITY;
						scd.Windowed = TRUE;
						scd.BufferDesc.RefreshRate.Numerator = 0; // change 0 to numerator for vsync
						scd.BufferDesc.RefreshRate.Denominator = 1; // change 1 to denominator for vynsc
						
						hr = dxgiFactory->CreateSwapChain(
							device,
							&scd,
							&swapchain
						);

						if (SUCCEEDED(hr))
						{
							dxgiFactory->Release();
							dxgiAdapter->Release();
							dxgiDevice->Release();
							return true;
						}
						dxgiFactory->Release();
					}
					dxgiAdapter->Release();	
				}
				dxgiDevice->Release();
			}
			LOG("Failed to create swapchain" << hr);
			return false;
		}

		bool D3d::CreateBackBuffer(ID3D11Device * device, IDXGISwapChain * swapchain, ID3D11RenderTargetView*& backBuffer, ID3D11ShaderResourceView*& backbufferSRV)
		{
			HRESULT hr;
			ID3D11Texture2D* pbackBuffer;
			hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pbackBuffer);


			D3D11_TEXTURE2D_DESC backBufferDesc;
			pbackBuffer->GetDesc(&backBufferDesc);


			if (FAILED(hr))
			{
				LOG_HR(hr);
				return false;
			}

			hr = device->CreateRenderTargetView(pbackBuffer, NULL, &backBuffer);

			if (FAILED(hr))
			{
				LOG_HR(hr);
				return false;
			}

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			srvDesc.Format = backBufferDesc.Format;
			srvDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = backBufferDesc.MipLevels;

			hr = device->CreateShaderResourceView(pbackBuffer, NULL, &backbufferSRV);

			if (FAILED(hr))
			{
				LOG_HR(hr);
				return false;
			}
			pbackBuffer->Release();
			return true;
		}

	
		ID3D11DepthStencilState* D3d::CreateDepthStencilState(D3D11_COMPARISON_FUNC func, bool depth)
		{
			ID3D11DepthStencilState* stencilState;
			CD3D11_DEPTH_STENCIL_DESC depthStencilDesc;
			ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

			depthStencilDesc.DepthEnable = depth;
			depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			depthStencilDesc.DepthFunc = func;
			depthStencilDesc.StencilReadMask = true;
			depthStencilDesc.StencilWriteMask = 0xFF;
			depthStencilDesc.StencilReadMask = 0xFF;

			// if front face
			depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
			depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			// if back face
			depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
			depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			HRESULT hr = ThomasCore::GetDevice()->CreateDepthStencilState(&depthStencilDesc, &stencilState);
			if (FAILED(hr))
			{
				LOG_HR(hr);
				return NULL;
			}
			return stencilState;
		}
		
		bool D3d::CreateDepthStencilView(ID3D11Device * device, LONG width, LONG height, ID3D11DepthStencilView *& stencilView, ID3D11DepthStencilView*& depthStencilViewReadOnly,
			ID3D11ShaderResourceView *& depthBufferSRV)
		{
			D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
			D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc;

			ZeroMemory(&depthStencilBufferDesc, sizeof(depthStencilBufferDesc));
			ZeroMemory(&depthViewDesc, sizeof(depthViewDesc));

			// Z-buffer texture desc
			depthStencilBufferDesc.Width = width;
			depthStencilBufferDesc.Height = height;
			depthStencilBufferDesc.MipLevels = 1;
			depthStencilBufferDesc.ArraySize = 1;
			depthStencilBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
			depthStencilBufferDesc.SampleDesc.Count = THOMAS_AA_COUNT;
			depthStencilBufferDesc.SampleDesc.Quality = THOMAS_AA_QUALITY;
			depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
			depthStencilBufferDesc.CPUAccessFlags = 0;
			depthStencilBufferDesc.MiscFlags = 0;
		

			ID3D11Texture2D* depthStencilBuffer;
			HRESULT hr = device->CreateTexture2D(&depthStencilBufferDesc, NULL, &depthStencilBuffer);
			if (FAILED(hr))
			{
				LOG_HR(hr);
				return false;
			}

			// Z-buffer view desc
			depthViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			#if THOMAS_AA_COUNT > 1
				depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
			#else
				depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			#endif
			depthViewDesc.Texture2D.MipSlice = 0;
			depthViewDesc.Flags = 0;

			


			hr = device->CreateDepthStencilView(depthStencilBuffer, &depthViewDesc, &stencilView);
			if (FAILED(hr))
			{
				LOG_HR(hr);
				stencilView = nullptr;
				return false;
			}
			depthViewDesc.Flags = D3D11_DSV_READ_ONLY_DEPTH | D3D11_DSV_READ_ONLY_STENCIL;
			hr = device->CreateDepthStencilView(depthStencilBuffer, &depthViewDesc, &depthStencilViewReadOnly);
			if (FAILED(hr))
			{
				LOG_HR(hr);
				stencilView = nullptr;
				return false;
			}

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory(&srvDesc, sizeof(srvDesc));
			srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			#if THOMAS_AA_COUNT > 1
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
			#else
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			#endif
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = -1;

			hr = device->CreateShaderResourceView(depthStencilBuffer, &srvDesc, &depthBufferSRV);
			if (FAILED(hr))
			{
				LOG_HR(hr);
				stencilView = nullptr;
				return false;
			}
			depthStencilBuffer->Release();
			return true;
			
		}

		ID3D11InfoQueue* D3d::CreateDebugInfoQueue() 
		{
			ID3D11InfoQueue* infoQueue;
			HRESULT hr = ThomasCore::GetDevice()->QueryInterface(IID_PPV_ARGS(&infoQueue));
			if (FAILED(hr))
			{
				LOG_HR(hr);
				return nullptr;
			}
			for (int i = 0; i < infoQueue->GetNumStoredMessages(); i++) {
				SIZE_T messageLength;	
				infoQueue->GetMessageA(i, NULL, &messageLength);
				D3D11_MESSAGE * message = (D3D11_MESSAGE*)malloc(messageLength);
				infoQueue->GetMessageA(i, message, &messageLength);
				LOG(message->pDescription);
			}
			infoQueue->ClearStoredMessages();
			return infoQueue;
		}

		bool D3d::LoadTextureFromFile(std::string fileName, 
			ID3D11Resource*& texture, ID3D11ShaderResourceView*& textureView)
		{
			// Convert from string to char in order to split by token
			// convert back from char, since its easier to compare strings than chars
			// Fuck you, thats why.
			char* filename_c = new char[fileName.length() + 1];
			std::strcpy(filename_c, fileName.c_str());

			char * extension_char = PathFindExtensionA(filename_c);
			std::string extension_string(extension_char);

			
			delete[] filename_c;

			HRESULT hr;
			if (extension_string == ".dds")
			{
				hr = DirectX::CreateDDSTextureFromFile(ThomasCore::GetDevice(), ThomasCore::GetDeviceContext(), CA2W(fileName.c_str()), &texture, &textureView);
			}
			else
			{
				hr = DirectX::CreateWICTextureFromFile(ThomasCore::GetDevice(), ThomasCore::GetDeviceContext(), CA2W(fileName.c_str()), &texture, &textureView);
			}
			
			if (FAILED(hr))
			{
				LOG("Failed to load texture: " << fileName << " error: ");
				LOG_HR(hr);
				return false;
			}

			return true;
		}

		bool D3d::LoadCubeTextureFromFile(std::string fileName, ID3D11Resource *& texture, ID3D11ShaderResourceView *& textureView)
		{
			char* filename_c = new char[fileName.length() + 1];
			std::strcpy(filename_c, fileName.c_str());

			char * extension_char = PathFindExtensionA(filename_c);
			std::string extension_string(extension_char);

			delete[] filename_c;

			HRESULT hr;
			if (extension_string == ".dds")
			{
				hr = DirectX::CreateDDSTextureFromFileEx(ThomasCore::GetDevice(), CA2W(fileName.c_str()), 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0,
					D3D11_RESOURCE_MISC_TEXTURECUBE, false, (ID3D11Resource**)&texture, &textureView, nullptr);
			}
	
			if (FAILED(hr))
			{
				LOG("Failed to load cubemap texture: " << fileName);
				LOG_HR(hr);
				return false;
			}

			return true;
		}

		ID3D11RasterizerState * D3d::CreateRasterizer(D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode)
		{
			ID3D11RasterizerState* rasterState;
			D3D11_RASTERIZER_DESC rasterDesc;

			rasterDesc.AntialiasedLineEnable = false;
			rasterDesc.CullMode = cullMode;
			rasterDesc.DepthBias = 0;
			rasterDesc.DepthBiasClamp = 0.0f;
			rasterDesc.DepthClipEnable = false;
			rasterDesc.FillMode = fillMode;
			rasterDesc.FrontCounterClockwise = true;
			rasterDesc.MultisampleEnable = false;
			rasterDesc.ScissorEnable = false;
			rasterDesc.SlopeScaledDepthBias = 0.0f;

			ThomasCore::GetDevice()->CreateRasterizerState(&rasterDesc, &rasterState);

			return rasterState;
		}
		
		void D3d::CreateTextureAndViews(UINT width, UINT height, DXGI_FORMAT format, ID3D11Texture2D *& tex, ID3D11ShaderResourceView *& SRV, ID3D11RenderTargetView *& RTV)
		{
			// Create 2D texture
			D3D11_TEXTURE2D_DESC tex_desc;
			tex_desc.Width = width;
			tex_desc.Height = height;
			tex_desc.MipLevels = 0;
			tex_desc.ArraySize = 1;
			tex_desc.Format = format;
			tex_desc.SampleDesc.Count = 1;
			tex_desc.SampleDesc.Quality = 0;
			tex_desc.Usage = D3D11_USAGE_DEFAULT;
			tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			tex_desc.CPUAccessFlags = 0;
			tex_desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

			ThomasCore::GetDevice()->CreateTexture2D(&tex_desc, NULL, &tex);

			// Create shader resource view
			tex->GetDesc(&tex_desc);
			if (tex)
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
				srv_desc.Format = format;
				srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srv_desc.Texture2D.MipLevels = tex_desc.MipLevels;
				srv_desc.Texture2D.MostDetailedMip = 0;

				ThomasCore::GetDevice()->CreateShaderResourceView(tex, &srv_desc, &SRV);
			}

			// Create render target view
			if (SRV)
			{
				D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
				rtv_desc.Format = format;
				rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				rtv_desc.Texture2D.MipSlice = 0;

				ThomasCore::GetDevice()->CreateRenderTargetView(tex, &rtv_desc, &RTV);
			}
		}

		void D3d::CreateBufferAndUAV(void * data, UINT byte_width, UINT byte_stride, ID3D11Buffer *& buffer, ID3D11UnorderedAccessView *& UAV, ID3D11ShaderResourceView *& SRV)
		{

			HRESULT result;
			// Create buffer
			D3D11_BUFFER_DESC buf_desc;
			buf_desc.ByteWidth = byte_width;
			buf_desc.Usage = D3D11_USAGE_DEFAULT;
			buf_desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
			buf_desc.CPUAccessFlags = 0;
			buf_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			buf_desc.StructureByteStride = byte_stride;

			D3D11_SUBRESOURCE_DATA init_data = { data, 0, 0 };

			result = ThomasCore::GetDevice()->CreateBuffer(&buf_desc, data != NULL ? &init_data : NULL, &buffer);
			if (FAILED(result))
			{
				LOG_HR(result);
			}

			// Create undordered access view
			D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
			uav_desc.Format = DXGI_FORMAT_UNKNOWN;
			uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			uav_desc.Buffer.FirstElement = 0;
			uav_desc.Buffer.NumElements = byte_width / byte_stride;
			uav_desc.Buffer.Flags = 0;

			result = ThomasCore::GetDevice()->CreateUnorderedAccessView(buffer, &uav_desc, &UAV);
			if (FAILED(result))
			{
				LOG_HR(result);
			}

			// Create shader resource view
			D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
			srv_desc.Format = DXGI_FORMAT_UNKNOWN;
			srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			srv_desc.Buffer.FirstElement = 0;
			srv_desc.Buffer.NumElements = byte_width / byte_stride;

			result = ThomasCore::GetDevice()->CreateShaderResourceView(buffer, &srv_desc, &SRV);
			if (FAILED(result))
			{
				LOG_HR(result);
			}
		}

		void D3d::CreateCPUReadBufferAndUAV(void * data, UINT byte_width, UINT byte_stride, ID3D11Buffer *& buffer, ID3D11UnorderedAccessView *& UAV)
		{
			HRESULT result;
			// Create buffer
			D3D11_BUFFER_DESC buf_desc;
			buf_desc.ByteWidth = byte_width;
			buf_desc.Usage = D3D11_USAGE_DEFAULT;
			buf_desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
			buf_desc.CPUAccessFlags = 0;
			buf_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			buf_desc.StructureByteStride = byte_stride;

			D3D11_SUBRESOURCE_DATA init_data = { data, 0, 0 };

			result = ThomasCore::GetDevice()->CreateBuffer(&buf_desc, data != NULL ? &init_data : NULL, &buffer);
			if (FAILED(result))
			{
				LOG_HR(result);
			}

			// Create undordered access view
			D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
			uav_desc.Format = DXGI_FORMAT_UNKNOWN;
			uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			uav_desc.Buffer.FirstElement = 0;
			uav_desc.Buffer.NumElements = byte_width / byte_stride;
			uav_desc.Buffer.Flags = 0;

			result = ThomasCore::GetDevice()->CreateUnorderedAccessView(buffer, &uav_desc, &UAV);
			if (FAILED(result))
			{
				LOG_HR(result);
			}
		}

		ID3D11Buffer * D3d::CreateStagingBuffer(UINT byte_width, UINT byte_stride)
		{
			HRESULT result;
			// Create buffer
			D3D11_BUFFER_DESC buf_desc;
			buf_desc.ByteWidth = byte_width;
			buf_desc.Usage = D3D11_USAGE_STAGING;
			buf_desc.BindFlags = 0;
			buf_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			buf_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			buf_desc.StructureByteStride = byte_stride;

			ID3D11Buffer* buffer;
			result = ThomasCore::GetDevice()->CreateBuffer(&buf_desc, NULL, &buffer);
			if (FAILED(result))
			{
				LOG_HR(result);
			}
			return buffer;
		}
	}
}