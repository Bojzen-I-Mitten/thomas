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
#include "../resource/Material.h"
#include "../resource/Shader.h"
#include "../resource/ShaderProperty.h"
#include <algorithm>
#include "../utils/DebugTools.h"
#include "../Window.h"


namespace thomas
{
	namespace graphics
	{
		CommandQueue Renderer::s_renderCommands;
		CommandQueue Renderer::s_lastFramesCommands;
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

		void Renderer::ClearCommands()
		{
			s_renderCommands.clear();
		}


		void Renderer::SubmitCommand(RenderCommand command)
		{
			s_renderCommands[command.camera][command.material].push_back(command);
		}

		void Renderer::TransferCommandList()
		{
			s_lastFramesCommands = s_renderCommands;
		}

		void Renderer::BindObject(thomas::resource::Material * material, thomas::math::Matrix& worldMatrix)
		{
			thomas::resource::ShaderProperty* prop;

			prop = material->GetProperty("thomas_ObjectToWorld");
			prop->SetMatrix(worldMatrix.Transpose());
			//prop->ApplyProperty(material->GetShader());

			prop = material->GetProperty("thomas_WorldToObject");
			prop->SetMatrix(worldMatrix.Invert());
			//prop->ApplyProperty(material->GetShader());

		}

		void Renderer::ProcessCommands()
		{
			BindFrame();
			for (auto& perCameraQueue : s_lastFramesCommands)
			{
				object::component::Camera* camera = perCameraQueue.first;
				BindCamera(camera);
				for (auto& perMaterialQueue : perCameraQueue.second)
				{
					resource::Material* material = perMaterialQueue.first;
					material->Bind();
					for (RenderCommand& perMeshCommand : perMaterialQueue.second)
					{
						BindObject(material, perMeshCommand.worldMatrix);
						material->Draw(perMeshCommand.mesh);
					}
				}
				
				
				
			}

		}

	}
}

