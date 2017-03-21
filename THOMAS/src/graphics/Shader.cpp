#include "Shader.h"
#include "../ThomasCore.h"
#include "../utils/d3d.h"
#include <AtlBase.h>
#include <atlconv.h>
#include "../Scene.h"

namespace thomas
{
	namespace graphics
	{

		Shader* Shader::s_currentBoundShader;
		std::vector<Shader*> Shader::s_loadedShaders;

		ID3DBlob* Shader::Compile(std::string source, std::string profile, std::string main)
		{
			ID3DBlob* shaderBlob;
			ID3DBlob* errorBlob;


			HRESULT status = D3DCompileFromFile(CA2W(source.c_str()), nullptr, nullptr, main.c_str(), profile.c_str(), D3DCOMPILE_DEBUG, 0, &shaderBlob, &errorBlob);


			if (status != S_OK)
			{

				if (errorBlob)
				{
					if (errorBlob->GetBufferSize())
					{
						std::string error((char*)errorBlob->GetBufferPointer());

#ifndef THOMAS_SHOW_ALL_ERRORS
						if (error.find("X3501") == std::string::npos) { //ignore annoying errors.
							LOG("SHADER ERROR : " << source << " errorBlob:" << error);
						}
#else
						LOG("SHADER ERROR : " << source << " errorBlob:" << error);
#endif

						errorBlob->Release();
					}
				}
				else
				{
					LOG("SHADER ERROR : " << source);
					LOG_HR(status);
				}
			}
			else if (status == S_OK)
			{
				if (errorBlob)
				{
					if (errorBlob->GetBufferSize())
					{
						//LOG("Shader Compiler : " << (char*)errorBlob->GetBufferPointer());
						errorBlob->Release();
					}
				}
				return shaderBlob;
			}

			return NULL;
		}

		bool Shader::CreateInputLayout(InputLayouts layout)
		{
			if (!m_data.vs)
				return false;
			std::vector<D3D11_INPUT_ELEMENT_DESC> layoutDesc;

			switch (layout)
			{
			case InputLayouts::STANDARD:
				layoutDesc =
				{
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
				};
				break;
			case InputLayouts::POST_EFFECT:
				layoutDesc = { { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 } };
				break;
			case InputLayouts::NONE:
				layoutDesc = { { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 } };
				break;
			default:
				return false;
				break;
			}


			HRESULT result = ThomasCore::GetDevice()->CreateInputLayout(&layoutDesc.front(), layoutDesc.size(), m_data.vs->GetBufferPointer(), m_data.vs->GetBufferSize(), &m_data.inputLayout);

			if (FAILED(result))
			{
				LOG("ERROR::FAILED TO CREATE INPUT LAYOUT");
				return false;
			}

			return true;
		}

		Shader::Shader(std::string name, InputLayouts inputLayout, std::string vertexShader, std::string geometryShader, std::string hullShader, std::string domainShader, std::string pixelShader, Scene* scene)
		{
			m_name = name;
			m_scene = scene;

			m_data.vs = NULL;
			m_data.vertexShader = nullptr;
			m_data.ps = NULL;
			m_data.pixelShader = nullptr;
			m_data.gs = NULL;
			m_data.geometryShader = nullptr;
			m_data.hs = NULL;
			m_data.hullShader = nullptr;
			m_data.ds = NULL;
			m_data.domainShader = nullptr;
			m_data.cs = NULL;
			m_data.computeShader = nullptr;

			if (!vertexShader.empty())
				m_data.vs = Compile(vertexShader, "vs_5_0", "VSMain");
			if (!geometryShader.empty())
				m_data.gs = Compile(geometryShader, "gs_5_0", "GSMain");
			if (!hullShader.empty())
				m_data.hs = Compile(hullShader, "hs_5_0", "HSMain");
			if (!domainShader.empty())
				m_data.ds = Compile(domainShader, "ds_5_0", "DSMain");
			if (!pixelShader.empty())
				m_data.ps = Compile(pixelShader, "ps_5_0", "PSMain");


			if (m_data.vs)
			{
				m_data.VSfilePath = vertexShader;
				ThomasCore::GetDevice()->CreateVertexShader(m_data.vs->GetBufferPointer(), m_data.vs->GetBufferSize(), NULL, &m_data.vertexShader);
			}
			if (m_data.ps)
			{
				m_data.PSfilePath = pixelShader;
				ThomasCore::GetDevice()->CreatePixelShader(m_data.ps->GetBufferPointer(), m_data.ps->GetBufferSize(), NULL, &m_data.pixelShader);
			}

			if (m_data.gs)
			{
				m_data.GSfilePath = geometryShader;
				ThomasCore::GetDevice()->CreateGeometryShader(m_data.gs->GetBufferPointer(), m_data.gs->GetBufferSize(), NULL, &m_data.geometryShader);
			}

			if (m_data.hs)
			{
				m_data.HSfilePath = hullShader;
				ThomasCore::GetDevice()->CreateHullShader(m_data.hs->GetBufferPointer(), m_data.hs->GetBufferSize(), NULL, &m_data.hullShader);
			}

			if (m_data.ds)
			{
				m_data.DSfilePath = domainShader;
				ThomasCore::GetDevice()->CreateDomainShader(m_data.ds->GetBufferPointer(), m_data.ds->GetBufferSize(), NULL, &m_data.domainShader);
			}

			CreateInputLayout(inputLayout);
		}

		Shader::Shader(std::string name, std::string ComputeShader, Scene * scene)
		{
			m_name = name;

			m_scene = scene;

			m_data.vs = NULL;
			m_data.vertexShader = nullptr;
			m_data.ps = NULL;
			m_data.pixelShader = nullptr;
			m_data.gs = NULL;
			m_data.geometryShader = nullptr;
			m_data.hs = NULL;
			m_data.hullShader = nullptr;
			m_data.ds = NULL;
			m_data.domainShader = nullptr;
			m_data.cs = NULL;
			m_data.computeShader = nullptr;
			m_data.inputLayout = NULL;
			m_data.cs = Compile(ComputeShader, "cs_5_0", "CSMain");

			if (m_data.cs)
			{
				m_data.CSFilePath = ComputeShader;
				ThomasCore::GetDevice()->CreateComputeShader(m_data.cs->GetBufferPointer(), m_data.cs->GetBufferSize(), NULL, &m_data.computeShader);
			}

		}




		Shader::Shader(std::string name, InputLayouts inputLayout, std::string filePath, Scene* scene)
		{
			m_name = name;
			
			m_scene = scene;

			m_data.vs = NULL;
			m_data.vertexShader = nullptr;
			m_data.ps = NULL;
			m_data.pixelShader = nullptr;
			m_data.gs = NULL;
			m_data.geometryShader = nullptr;
			m_data.hs = NULL;
			m_data.hullShader = nullptr;
			m_data.ds = NULL;
			m_data.domainShader = nullptr;
			m_data.cs = NULL;
			m_data.computeShader = nullptr;

			m_data.vs = Compile(filePath, "vs_5_0", "VSMain");
			m_data.ps = Compile(filePath, "ps_5_0", "PSMain");
			m_data.gs = Compile(filePath, "gs_5_0", "GSMain");
			m_data.hs = Compile(filePath, "hs_5_0", "HSMain");
			m_data.ds = Compile(filePath, "ds_5_0", "DSMain");

			if (m_data.vs)
			{
				m_data.VSfilePath = filePath;
				ThomasCore::GetDevice()->CreateVertexShader(m_data.vs->GetBufferPointer(), m_data.vs->GetBufferSize(), NULL, &m_data.vertexShader);
			}
			if (m_data.ps)
			{
				m_data.PSfilePath = filePath;
				ThomasCore::GetDevice()->CreatePixelShader(m_data.ps->GetBufferPointer(), m_data.ps->GetBufferSize(), NULL, &m_data.pixelShader);
			}
				
			if (m_data.gs)
			{
				m_data.GSfilePath = filePath;
				ThomasCore::GetDevice()->CreateGeometryShader(m_data.gs->GetBufferPointer(), m_data.gs->GetBufferSize(), NULL, &m_data.geometryShader);
			}
				
			if (m_data.hs)
			{
				m_data.HSfilePath = filePath;
				ThomasCore::GetDevice()->CreateHullShader(m_data.hs->GetBufferPointer(), m_data.hs->GetBufferSize(), NULL, &m_data.hullShader);
			}
				
			if (m_data.ds)
			{
				m_data.DSfilePath = filePath;
				ThomasCore::GetDevice()->CreateDomainShader(m_data.ds->GetBufferPointer(), m_data.ds->GetBufferSize(), NULL, &m_data.domainShader);
			}

			CreateInputLayout(inputLayout);

		}

		Shader::~Shader()
		{
			Unbind();
			SAFE_RELEASE(m_data.vertexShader);
			SAFE_RELEASE(m_data.vs);

			SAFE_RELEASE(m_data.vertexShader);
			SAFE_RELEASE(m_data.vs);

			SAFE_RELEASE(m_data.pixelShader);
			SAFE_RELEASE(m_data.ps);

			SAFE_RELEASE(m_data.geometryShader);
			SAFE_RELEASE(m_data.gs);

			SAFE_RELEASE(m_data.hullShader);
			SAFE_RELEASE(m_data.hs);

			SAFE_RELEASE(m_data.domainShader);
			SAFE_RELEASE(m_data.ds);

			SAFE_RELEASE(m_data.computeShader);
			SAFE_RELEASE(m_data.cs);
			
			SAFE_RELEASE(m_data.inputLayout);

		}

		bool Shader::Destroy() {
			for (unsigned int i = 0; i < s_loadedShaders.size(); i++)
			{
				delete s_loadedShaders[i];
			}
			delete s_currentBoundShader;
			s_currentBoundShader = NULL;
			s_loadedShaders.clear();
			return true;
		}

		bool Shader::Bind()
		{
			if (s_currentBoundShader == this)
				return true;
			s_currentBoundShader = this;
			if (m_data.vs)
			{
				ThomasCore::GetDeviceContext()->IASetInputLayout(m_data.inputLayout);
				ThomasCore::GetDeviceContext()->VSSetShader(m_data.vertexShader, NULL, 0);
			}

			if (m_data.ps)
				ThomasCore::GetDeviceContext()->PSSetShader(m_data.pixelShader, NULL, 0);

			if (m_data.gs)
				ThomasCore::GetDeviceContext()->GSSetShader(m_data.geometryShader, NULL, 0);

			if (m_data.hs)
				ThomasCore::GetDeviceContext()->HSSetShader(m_data.hullShader, NULL, 0);

			if (m_data.ds)
				ThomasCore::GetDeviceContext()->DSSetShader(m_data.domainShader, NULL, 0);
			if (m_data.cs)
				ThomasCore::GetDeviceContext()->CSSetShader(m_data.computeShader, NULL, 0);
			return true;
		}
		bool Shader::Unbind()
		{
			ThomasCore::GetDeviceContext()->VSSetShader(NULL, NULL, 0);
			ThomasCore::GetDeviceContext()->PSSetShader(NULL, NULL, 0);
			ThomasCore::GetDeviceContext()->GSSetShader(NULL, NULL, 0);
			ThomasCore::GetDeviceContext()->HSSetShader(NULL, NULL, 0);
			ThomasCore::GetDeviceContext()->DSSetShader(NULL, NULL, 0);
			ThomasCore::GetDeviceContext()->CSSetShader(NULL, NULL, 0);
			if(s_currentBoundShader == this)
				s_currentBoundShader = nullptr;
			return true;
		}
		std::string Shader::GetName()
		{
			return m_name;
		}

		bool Shader::BindBuffer(ID3D11Buffer * resource, ResourceType type)
		{
			if (s_currentBoundShader == this)
			{
				return BindBuffer(resource, (int)type);
			}
			return false;
		}
		bool Shader::BindBuffer(ID3D11Buffer * resource, int slot)
		{
			if (s_currentBoundShader && s_currentBoundShader == this)
			{
				if (m_data.vs)
					ThomasCore::GetDeviceContext()->VSSetConstantBuffers(slot, 1, &resource);
				if (m_data.ps)
					ThomasCore::GetDeviceContext()->PSSetConstantBuffers(slot, 1, &resource);
				if (m_data.gs)
					ThomasCore::GetDeviceContext()->GSSetConstantBuffers(slot, 1, &resource);
				if (m_data.hs)
					ThomasCore::GetDeviceContext()->HSSetConstantBuffers(slot, 1, &resource);
				if (m_data.ds)
					ThomasCore::GetDeviceContext()->DSSetConstantBuffers(slot, 1, &resource);
				if (m_data.cs)
					ThomasCore::GetDeviceContext()->CSSetConstantBuffers(slot, 1, &resource);
				return true;
			}
			return false;
		}
		bool Shader::BindUAV(ID3D11UnorderedAccessView * uav, int slot)
		{
			if (s_currentBoundShader && s_currentBoundShader == this)
			{
				if (m_data.cs)
				{
					ThomasCore::GetDeviceContext()->CSSetUnorderedAccessViews(slot, 1, &uav, NULL);
				}
				return true;
			}
			
			return false;
		}
		bool Shader::BindResource(ID3D11ShaderResourceView * texture, int slot)
		{
			if (s_currentBoundShader && s_currentBoundShader == this)
			{
				if (m_data.vs)
					ThomasCore::GetDeviceContext()->VSSetShaderResources(slot, 1, &texture);
				if (m_data.ps)
					ThomasCore::GetDeviceContext()->PSSetShaderResources(slot, 1, &texture);
				if (m_data.gs)
					ThomasCore::GetDeviceContext()->GSGetShaderResources(slot, 1, &texture);
				if (m_data.hs)
					ThomasCore::GetDeviceContext()->HSSetShaderResources(slot, 1, &texture);
				if (m_data.ds)
					ThomasCore::GetDeviceContext()->DSSetShaderResources(slot, 1, &texture);
				if (m_data.cs)
					ThomasCore::GetDeviceContext()->CSSetShaderResources(slot, 1, &texture);
				return true;
			}
			return false;
		}
		bool Shader::BindTextureSampler(ID3D11SamplerState * sampler, int slot)
		{
			if (s_currentBoundShader && s_currentBoundShader == this)
			{
				if (m_data.vs)
					ThomasCore::GetDeviceContext()->VSSetSamplers(slot, 1, &sampler);
				if (m_data.ps)
					ThomasCore::GetDeviceContext()->PSSetSamplers(slot, 1, &sampler);
				if (m_data.gs)
					ThomasCore::GetDeviceContext()->GSSetSamplers(slot, 1, &sampler);
				if (m_data.hs)
					ThomasCore::GetDeviceContext()->HSSetSamplers(slot, 1, &sampler);
				if (m_data.ds)
					ThomasCore::GetDeviceContext()->DSSetSamplers(slot, 1, &sampler);
				if (m_data.cs)
					ThomasCore::GetDeviceContext()->CSSetSamplers(slot, 1, &sampler);
				return true;
			}
			return false;
		}
		bool Shader::BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY type)
		{
			if (s_currentBoundShader == this && m_data.vs)
				ThomasCore::GetDeviceContext()->IASetPrimitiveTopology(type);
			return true;
		}
		bool Shader::BindVertexBuffer(ID3D11Buffer * vertexBuffer, UINT stride, UINT offset = 0)
		{
			if (s_currentBoundShader == this && m_data.vs)
				ThomasCore::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
			return true;
		}
		bool Shader::BindIndexBuffer(ID3D11Buffer * indexBuffer)
		{
			if (s_currentBoundShader == this && m_data.vs)
				ThomasCore::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
			return true;
		}




		void Shader::ReloadShader()
		{
			ID3D10Blob* tempBlob;
			if (m_data.vs)
			{		
				tempBlob = Compile(m_data.VSfilePath, "vs_5_0", "VSMain");
				if (tempBlob)
				{
					SAFE_RELEASE(m_data.vs);
					SAFE_RELEASE(m_data.vertexShader);
					m_data.vs = tempBlob;
					ThomasCore::GetDevice()->CreateVertexShader(m_data.vs->GetBufferPointer(), m_data.vs->GetBufferSize(), NULL, &m_data.vertexShader);
				}
					
			}
			if (m_data.ps)
			{		
				tempBlob = Compile(m_data.PSfilePath, "ps_5_0", "PSMain");
				if (tempBlob)
				{
					SAFE_RELEASE(m_data.ps);
					SAFE_RELEASE(m_data.pixelShader);
					m_data.ps = tempBlob;
					ThomasCore::GetDevice()->CreatePixelShader(m_data.ps->GetBufferPointer(), m_data.ps->GetBufferSize(), NULL, &m_data.pixelShader);
				}
					
			}
			if (m_data.gs)
			{		
				tempBlob = Compile(m_data.GSfilePath, "gs_5_0", "GSMain");
				if (tempBlob)
				{
					SAFE_RELEASE(m_data.gs);
					SAFE_RELEASE(m_data.geometryShader);
					m_data.gs = tempBlob;
					ThomasCore::GetDevice()->CreateGeometryShader(m_data.gs->GetBufferPointer(), m_data.gs->GetBufferSize(), NULL, &m_data.geometryShader);
				}
					
			}
			if (m_data.ds)
			{	
				tempBlob = Compile(m_data.DSfilePath, "ds_5_0", "DSMain");
				if (tempBlob)
				{
					SAFE_RELEASE(m_data.ds);
					SAFE_RELEASE(m_data.domainShader);
					m_data.ds = tempBlob;
					ThomasCore::GetDevice()->CreateDomainShader(m_data.ds->GetBufferPointer(), m_data.ds->GetBufferSize(), NULL, &m_data.domainShader);
				}
					
			}
			if (m_data.hs)
			{			
				tempBlob = Compile(m_data.HSfilePath, "hs_5_0", "HSMain");
				if (tempBlob)
				{
					SAFE_RELEASE(m_data.hs);
					SAFE_RELEASE(m_data.hullShader);
					m_data.hs = tempBlob;
					ThomasCore::GetDevice()->CreateHullShader(m_data.hs->GetBufferPointer(), m_data.hs->GetBufferSize(), NULL, &m_data.hullShader);
				}
					
			}
			if (m_data.cs)
			{
				tempBlob = Compile(m_data.CSFilePath, "cs_5_0", "CSMain");
				if (tempBlob)
				{
					SAFE_RELEASE(m_data.cs);
					SAFE_RELEASE(m_data.computeShader);
					m_data.cs = tempBlob;
					ThomasCore::GetDevice()->CreateComputeShader(m_data.cs->GetBufferPointer(), m_data.cs->GetBufferSize(), NULL, &m_data.computeShader);
				}

			}
		}

		
		Shader * Shader::CreateShader(std::string name, InputLayouts inputLayout, std::string filePath, Scene* scene)
		{
			Shader* shader;
			if (inputLayout == InputLayouts::POST_EFFECT)
			{
				shader = new Shader(name, inputLayout, "../res/thomasShaders/postEffect.hlsl", "", "", "", filePath, scene);
			}
			else if (inputLayout == InputLayouts::NONE)
			{
				shader = new Shader(name, inputLayout, filePath, "", "", "", filePath, scene);
				
			}
			else
			{
				shader = new Shader(name, inputLayout, filePath, scene);
			}
			if (shader)
				s_loadedShaders.push_back(shader);
			return shader;
		}
		Shader * Shader::CreateShader(std::string name, InputLayouts inputLayout, std::string vertexShader, std::string geometryShader, std::string hullShader, std::string domainShader, std::string pixelShader, Scene* scene)
		{
			Shader* shader = new Shader(name, inputLayout, vertexShader, geometryShader, hullShader, domainShader, pixelShader, scene);
			if (shader)
				s_loadedShaders.push_back(shader);

			return shader;
		}
		Shader * Shader::CreateComputeShader(std::string name, std::string computeShader, Scene * scene)
		{
			Shader* shader = new Shader(name, computeShader, scene);
			if (shader)
				s_loadedShaders.push_back(shader);
			return shader;
		}
		Shader * Shader::GetCurrentBoundShader()
		{
			return s_currentBoundShader;
		}
		Shader * Shader::GetShaderByName(std::string name)
		{
			for (unsigned int i = 0; i < s_loadedShaders.size(); i++)
			{
				if (s_loadedShaders[i]->GetName() == name)
					return s_loadedShaders[i];
			}
			return NULL;
		}
		std::vector<Shader*> Shader::GetShadersByScene(Scene * scene)
		{
			std::vector<Shader*> output;
			for (Shader* shader : s_loadedShaders)
				if (shader->m_scene == scene)
					output.push_back(shader);
			return output;
		}
		std::vector<Shader*> Shader::GetLoadedShaders()
		{
			return s_loadedShaders;
		}
		void Shader::ReloadShaders()
		{
			for (int i = 0; i < s_loadedShaders.size(); ++i)
			{
				s_loadedShaders[i]->ReloadShader();
			}
		}
		void Shader::Destroy(Scene* scene)
		{
			for (int i = 0; i < s_loadedShaders.size(); ++i)
				if (s_loadedShaders[i]->m_scene == scene)
				{
					delete s_loadedShaders[i];
					s_loadedShaders.erase(s_loadedShaders.begin() + i);
					i -= 1;
				}
		}
	}

}