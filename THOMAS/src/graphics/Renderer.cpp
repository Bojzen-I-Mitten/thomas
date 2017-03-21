#include "Renderer.h"
#include "Model.h"
#include "../object/component/Light.h"
#include "../Scene.h"
#include "LightManager.h"
#include "TextRender.h"
#include "Sprite.h"
#include "PostEffect.h"
#include "../utils/d3d.h"

namespace thomas
{
	namespace graphics
	{
		
		ID3D11RenderTargetView* Renderer::s_backBuffer = NULL;
		ID3D11ShaderResourceView* Renderer::s_backBufferSRV = NULL;
		ID3D11RasterizerState* Renderer::s_rasterState = NULL;
		ID3D11RasterizerState* Renderer::s_wireframeRasterState = NULL;
		ID3D11DepthStencilState* Renderer::s_depthStencilState = NULL;
		ID3D11DepthStencilView* Renderer::s_depthStencilView = NULL;
		ID3D11ShaderResourceView* Renderer::s_depthBufferSRV = NULL;
		ID3D11DepthStencilView* Renderer::s_depthStencilViewReadOnly = NULL;

		ID3D11Buffer* Renderer::s_objectBuffer;
		Renderer::GameObjectBuffer Renderer::s_objectBufferStruct;

		bool thomas::graphics::Renderer::Init()
		{

			if (utils::D3d::InitRenderer(s_backBuffer, s_backBufferSRV, s_depthStencilState, s_depthStencilView, s_depthStencilViewReadOnly, s_depthBufferSRV))
			{
				s_objectBuffer = utils::D3d::CreateDynamicBufferFromStruct(s_objectBufferStruct, D3D11_BIND_CONSTANT_BUFFER);
				s_rasterState = utils::D3d::CreateRasterizer(D3D11_FILL_SOLID, D3D11_CULL_BACK);
				s_wireframeRasterState = utils::D3d::CreateRasterizer(D3D11_FILL_WIREFRAME, D3D11_CULL_BACK);
				return true;

			}
			return false;
		}

		void thomas::graphics::Renderer::Clear()
		{
			float color[4] = { 0.0f, 0.5f, 0.75f, 1.0f };
			ThomasCore::GetDeviceContext()->ClearRenderTargetView(s_backBuffer, color);
			ThomasCore::GetDeviceContext()->ClearDepthStencilView(s_depthStencilView, D3D11_CLEAR_DEPTH, 1, 0);

		}

		void thomas::graphics::Renderer::Render()
		{
			Scene::Render();

			for (PostEffect* fx : PostEffect::GetLoadedPostEffects())
			{
				fx->Update();
			}

			if (Input::GetKey(Input::Keys::X))
			{
				ThomasCore::GetDeviceContext()->RSSetState(s_wireframeRasterState);
			}
			else
			{
				ThomasCore::GetDeviceContext()->RSSetState(s_rasterState);
			}

			ThomasCore::GetSwapChain()->Present(0, 0);
		}

		bool Renderer::Destroy()
		{
			SAFE_RELEASE(s_backBuffer);
			SAFE_RELEASE(s_backBufferSRV);
			SAFE_RELEASE(s_rasterState);
			SAFE_RELEASE(s_wireframeRasterState);
			SAFE_RELEASE(s_depthStencilState);
			SAFE_RELEASE(s_depthStencilView);
			SAFE_RELEASE(s_depthStencilViewReadOnly);
			SAFE_RELEASE(s_depthBufferSRV);
			SAFE_RELEASE(s_objectBuffer);
			
			return true;
		}
		std::vector<object::component::Camera*> Renderer::GetCameras()
		{
			std::vector<object::component::Camera*> cameras;
			for (object::GameObject* gameObject : object::GameObject::FindGameObjectsWithComponent<object::component::Camera>())
			{
				cameras.push_back(gameObject->GetComponent<object::component::Camera>());
			}
			return cameras;
		}
		void Renderer::UpdateGameObjectBuffer(object::component::Camera * camera, object::GameObject * gameObject)
		{
			//Fill matrix buffer with gameObject info

			s_objectBufferStruct.worldMatrix = gameObject->m_transform->GetWorldMatrix().Transpose();
			s_objectBufferStruct.viewMatrix = camera->GetViewMatrix().Transpose();
			s_objectBufferStruct.projectionMatrix = camera->GetProjMatrix().Transpose();
			s_objectBufferStruct.mvpMatrix = s_objectBufferStruct.projectionMatrix * s_objectBufferStruct.viewMatrix * s_objectBufferStruct.worldMatrix;
			s_objectBufferStruct.camPos = camera->GetPosition();
			s_objectBufferStruct.camDir = camera->m_gameObject->m_transform->Forward();

			utils::D3d::FillDynamicBufferStruct(s_objectBuffer, s_objectBufferStruct);

			
		}

		void Renderer::BindGameObjectBuffer()
		{
			thomas::graphics::Shader::GetCurrentBoundShader()->BindBuffer(s_objectBuffer, thomas::graphics::Shader::ResourceType::GAME_OBJECT);
		}

		ID3D11ShaderResourceView* Renderer::GetDepthBufferSRV()
		{
			return s_depthBufferSRV;
		}
		ID3D11RenderTargetView * Renderer::GetBackBuffer()
		{
			return s_backBuffer;
		}
		void Renderer::UnBindGameObjectBuffer()
		{
			thomas::graphics::Shader::GetCurrentBoundShader()->BindBuffer(NULL, thomas::graphics::Shader::ResourceType::GAME_OBJECT);
		}
		void Renderer::RenderSetup(object::component::Camera* camera)
		{
			ThomasCore::GetDeviceContext()->OMSetRenderTargets(1, &s_backBuffer, s_depthStencilView);
			if(camera)
				ThomasCore::GetDeviceContext()->RSSetViewports(1, camera->GetViewport().Get11());

			ThomasCore::GetDeviceContext()->OMSetDepthStencilState(s_depthStencilState, 1);
			ThomasCore::GetDeviceContext()->RSSetState(s_rasterState);
		}
		void Renderer::BindDepthNormal()
		{
			ThomasCore::GetDeviceContext()->OMSetRenderTargets(1, &s_backBuffer, s_depthStencilView);
		}
		void Renderer::BindDepthReadOnly()
		{
			ThomasCore::GetDeviceContext()->OMSetRenderTargets(1, &s_backBuffer, s_depthStencilViewReadOnly);
		}
		void Renderer::BindDepthBufferTexture()
		{
			BindDepthReadOnly();
			ID3D11SamplerState* sampler = Texture::GetSamplerState(Texture::SamplerState::WRAP);
			ThomasCore::GetDeviceContext()->PSSetSamplers(5, 1, &sampler);
			ThomasCore::GetDeviceContext()->PSSetShaderResources(5, 1, &s_depthBufferSRV);
		}
		void Renderer::UnbindDepthBufferTexture()
		{
			ID3D11ShaderResourceView* nullSRV = NULL;
			ThomasCore::GetDeviceContext()->PSSetShaderResources(5, 1, &nullSRV);
			BindDepthNormal();
		}
		void Renderer::ResetDepthStencilState()
		{
			ThomasCore::GetDeviceContext()->OMSetDepthStencilState(s_depthStencilState, 1);
		}
	}
}

