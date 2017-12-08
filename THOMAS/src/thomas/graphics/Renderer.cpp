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
#include "../Window.h"
namespace thomas
{
	namespace graphics
	{
		
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
			ThomasCore::GetDeviceContext()->RSSetViewports(1, camera->GetViewport().Get11());
			//ThomasPerCamera
			Shader::SetGlobalMatrix("thomas_MatrixP", camera->GetProjMatrix().Transpose());
			Shader::SetGlobalMatrix("thomas_MatrixV", camera->GetViewMatrix().Transpose());
			Shader::SetGlobalMatrix("thomas_MatrixInvV", camera->GetViewMatrix().Invert());
			Shader::SetGlobalMatrix("thomas_MatrixVP", camera->GetViewProjMatrix().Transpose());
			
			Shader::SetGlobalVector("_WorldSpaceCameraPos", (math::Vector4)camera->GetPosition());
		}

		void Renderer::BindPerObject(thomas::graphics::Material * material, thomas::object::component::Transform * transform)
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
			BindPerFrame();
		}

		void Renderer::RenderCamera(thomas::object::component::Camera * camera)
		{
			int displayIndex = camera->GetTargetDisplayIndex();
			Window::GetWindow(displayIndex)->Bind();
			BindPerCamera(camera);
			RenderQueue(Scene::GetCurrentScene()->GetRenderQueue());

			Physics::DrawDebug(camera);
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
				BindPerObject(lastMaterial, renderPair->transform);

				lastMaterial->Draw(renderPair->mesh);
			}

		}

		bool Renderer::SortPairs(RenderPair* a, RenderPair* b)
		{
			return a->material->GetId() < b->material->GetId();
		}

	}
}

