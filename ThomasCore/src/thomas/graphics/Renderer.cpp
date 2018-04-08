#include "Renderer.h"
#include "../object/component/Light.h"
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
#include "../Window.h"
namespace thomas
{
	namespace graphics
	{
		
		std::vector<graphics::RenderPair*> Renderer::s_renderQueue;
		std::vector<graphics::RenderPair*> Renderer::s_lastFramesRenederQueue;
		void Renderer::BindFrame()
		{
			//ThomasPerFrame
			float realDeltaTime = ThomasTime::GetActualDeltaTime();
			float dt = ThomasTime::GetDeltaTime();
			math::Vector4 thomas_DeltaTime(realDeltaTime, 1 / realDeltaTime, dt, 1 / dt);

			resource::Shader::SetGlobalVector("thomas_DeltaTime", thomas_DeltaTime);
		}

		void Renderer::BindCamera(thomas::object::component::Camera * camera)
		{
			int displayIndex = camera->GetTargetDisplayIndex();
			Window* window = Window::GetWindow(displayIndex);
			if (!window || !window->Initialized())
				return;

			window->Bind();
			ThomasCore::GetDeviceContext()->RSSetViewports(1, camera->GetViewport().Get11());

			//ThomasPerCamera
			resource::Shader::SetGlobalMatrix("thomas_MatrixP", camera->GetProjMatrix().Transpose());
			resource::Shader::SetGlobalMatrix("thomas_MatrixV", camera->GetViewMatrix().Transpose());
			resource::Shader::SetGlobalMatrix("thomas_MatrixInvV", camera->GetViewMatrix().Invert());
			resource::Shader::SetGlobalMatrix("thomas_MatrixVP", camera->GetViewProjMatrix().Transpose());
			
			resource::Shader::SetGlobalVector("_WorldSpaceCameraPos", (math::Vector4)camera->GetPosition());
		}

		void Renderer::ClearRenderQueue()
		{
			s_renderQueue.clear();
		}

		void Renderer::AddToRenderQueue(graphics::RenderPair * renderPair)
		{
			s_renderQueue.push_back(renderPair);
		}

		std::vector<graphics::RenderPair*> Renderer::GetRenderQueue()
		{
			return s_renderQueue;
		}

		void Renderer::BindObject(thomas::graphics::Material * material, thomas::object::component::Transform * transform)
		{
			thomas::graphics::MaterialProperty* prop;

			prop = material->GetProperty("thomas_ObjectToWorld");
			prop->SetMatrix(transform->GetWorldMatrix().Transpose());
			prop->ApplyProperty(material->GetShader());

			prop = material->GetProperty("thomas_WorldToObject");
			prop->SetMatrix(transform->GetWorldMatrix().Invert());
			prop->ApplyProperty(material->GetShader());

		}

		void Renderer::Begin()
		{
			BindFrame();
		}

		void Renderer::Render()
		{
			
			RenderQueue(s_renderQueue);
		}

		void Renderer::RenderQueue(std::vector<RenderPair*> renderQueue)
		{
			std::sort(renderQueue.begin(), renderQueue.end(), SortPairs);

			Material* lastMaterial = nullptr;
			for (RenderPair* renderPair : renderQueue)
			{
				if (!lastMaterial || lastMaterial != renderPair->material)
				{
					lastMaterial = renderPair->material;
					lastMaterial->Bind();
				}
				BindObject(lastMaterial, renderPair->transform);

				lastMaterial->Draw(renderPair->mesh);
			}

		}

		bool Renderer::SortPairs(RenderPair* a, RenderPair* b)
		{
			return a->material->GetId() < b->material->GetId();
		}

	}
}

