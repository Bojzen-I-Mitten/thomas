#include "Renderer.h"
#include "Model.h"
#include "../object/component/Light.h"
#include "../Scene.h"
#include "LightManager.h"
#include "TextRender.h"
#include "Sprite.h"
#include "../utils/d3d.h"

#include "../object/GameObject.h"
#include "../object/component/Camera.h"
#include "../ThomasCore.h"
#include "Texture.h"
#include "../object/component/Transform.h"
#include "../Input.h"

#include "../ThomasTime.h"
#include "Material.h"
#include <algorithm>
#include "../utils/DebugTools.h"

namespace thomas
{
	namespace graphics
	{
		
		ID3D11RenderTargetView* Renderer::s_backBuffer = NULL;
		ID3D11ShaderResourceView* Renderer::s_backBufferSRV = NULL;
		ID3D11DepthStencilState* Renderer::s_depthStencilState = NULL;
		ID3D11DepthStencilView* Renderer::s_depthStencilView = NULL;
		ID3D11ShaderResourceView* Renderer::s_depthBufferSRV = NULL;
		ID3D11DepthStencilView* Renderer::s_depthStencilViewReadOnly = NULL;


		bool thomas::graphics::Renderer::Init()
		{

			return (utils::D3d::InitRenderer(s_backBuffer, s_backBufferSRV, s_depthStencilState, s_depthStencilView, s_depthStencilViewReadOnly, s_depthBufferSRV));
		}

		bool Renderer::Resize()
		{
			ThomasCore::GetDeviceContext()->OMSetRenderTargets(0, 0, 0);
			SAFE_RELEASE(s_backBuffer);
			SAFE_RELEASE(s_backBufferSRV);
			SAFE_RELEASE(s_depthStencilView);
			SAFE_RELEASE(s_depthStencilState);
			SAFE_RELEASE(s_depthStencilViewReadOnly);
			SAFE_RELEASE(s_depthBufferSRV);	

			ThomasCore::GetSwapChain()->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
			ID3D11Texture2D* pBuffer;

			utils::D3d::InitRenderer(s_backBuffer, s_backBufferSRV, s_depthStencilState, s_depthStencilView, s_depthStencilViewReadOnly, s_depthBufferSRV);
					
			return true;
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
						
		}

		bool Renderer::Destroy()
		{
			SAFE_RELEASE(s_backBuffer);
			SAFE_RELEASE(s_backBufferSRV);
			SAFE_RELEASE(s_depthStencilState);
			SAFE_RELEASE(s_depthStencilView);
			SAFE_RELEASE(s_depthStencilViewReadOnly);
			SAFE_RELEASE(s_depthBufferSRV);
			
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
	

		ID3D11ShaderResourceView* Renderer::GetDepthBufferSRV()
		{
			return s_depthBufferSRV;
		}
		ID3D11RenderTargetView * Renderer::GetBackBuffer()
		{
			return s_backBuffer;
		}

		void Renderer::BindPerFrame()
		{
			//ThomasPerFrame
			float realDeltaTime = ThomasTime::GetActualDeltaTime();
			float dt = ThomasTime::GetDeltaTime();
			math::Vector4 thomas_DeltaTime(realDeltaTime, 1 / realDeltaTime, dt, 1 / dt);

			Shader::SetGlobalVector("thomas_DeltaTime", thomas_DeltaTime);
		}

		void Renderer::BindPerCamera(thomas::object::component::Camera * camera)
		{
			ThomasCore::GetDeviceContext()->OMSetRenderTargets(1, &s_backBuffer, s_depthStencilView);
			ThomasCore::GetDeviceContext()->RSSetViewports(1, camera->GetViewport().Get11());
			//ThomasPerCamera
			Shader::SetGlobalMatrix("thomas_CameraProjection", camera->GetProjMatrix());
			Shader::SetGlobalMatrix("thomas_MatrixV", camera->GetViewMatrix());
			Shader::SetGlobalMatrix("thomas_MatrixInvV", camera->GetViewMatrix().Invert());
			Shader::SetGlobalMatrix("thomas_MatrixVP", camera->GetViewProjMatrix());
			
			Shader::SetGlobalVector("_WorldSpaceCameraPos", (math::Vector4)camera->GetPosition());
		}

		void Renderer::BindPerObject(thomas::graphics::Material * material, thomas::object::component::Transform * transform)
		{
			thomas::graphics::MaterialProperty* prop;

			prop = material->GetProperty("thomas_ObjectToWorld");
			prop->SetMatrix(transform->GetWorldMatrix());
			prop->ApplyProperty(material->GetShader());

			prop = material->GetProperty("thomas_WorldToObject");
			prop->SetMatrix(transform->GetWorldMatrix().Invert());
			prop->ApplyProperty(material->GetShader());

		}

		void Renderer::Render(Scene * scene)
		{
			Clear();
			ThomasCore::GetDeviceContext()->OMSetRenderTargets(1, &s_backBuffer, s_depthStencilView);
			ThomasCore::GetDeviceContext()->OMSetDepthStencilState(s_depthStencilState, 1);
			BindPerFrame();

			std::vector<object::component::Camera*> cameras = object::Object::FindObjectsOfType<object::component::Camera>();
			for (object::component::Camera* camera : cameras)
			{
				ThomasCore::GetDeviceContext()->RSSetViewports(1, camera->GetViewport().Get11());
				BindPerCamera(camera);
				RenderQueue(scene->GetRenderQueue());

				Physics::DrawDebug(camera);
				
			}
			utils::DebugTools::Draw();
			ThomasCore::GetSwapChain()->Present(0, 0);
						
		}

		void Renderer::RenderQueue(std::vector<RenderPair*> renderQueue)
		{

			std::sort(renderQueue.begin(), renderQueue.end(), SortPairs);

			Material* lastMaterial;
			for (RenderPair* renderPair : renderQueue)
			{
				if (lastMaterial != renderPair->material)
				{
					lastMaterial = renderPair->material;
					lastMaterial->Bind();
				}
				BindPerObject(lastMaterial, renderPair->transform);

				lastMaterial->Draw(renderPair->mesh);
			}
		}

		bool Renderer::SortPairs(RenderPair* a, RenderPair* b)
		{
			return a->material->GetId() < a->material->GetId();
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

