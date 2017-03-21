#include "PostEffect.h"
#include "../Scene.h"
#include "../utils/d3d.h"
namespace thomas
{
	namespace graphics
	{
		std::vector<PostEffect*> PostEffect::s_loadedEffects;
		std::map<std::string, PostEffect*> PostEffect::s_postEffectTypes;
		ID3D11Buffer* PostEffect::s_quadVertexBuffer;
		PostEffect* PostEffect::s_renderToBackBuffer;
		PostEffect::CameraBufferStruct PostEffect::s_cameraBufferStruct;
		ID3D11Buffer* PostEffect::s_cameraBuffer;
		void PostEffect::UpdateCameraBuffer(object::component::Camera * camera)
		{
			s_cameraBufferStruct.camPos = camera->GetPosition();
			s_cameraBufferStruct.viewMatrixInv = camera->GetViewMatrix().Invert().Transpose();
			s_cameraBufferStruct.projectionMatrixInv = camera->GetProjMatrix().Invert().Transpose();

			utils::D3d::FillDynamicBufferStruct(s_cameraBuffer, s_cameraBufferStruct);
		}
		PostEffect::PostEffect(std::string shader)
		{
			m_shader = Shader::GetShaderByName(shader);
		}

		PostEffect::PostEffect(std::string name, Shader* shader)
		{
			m_shader = shader;
			m_name = name;
			m_effectProperties = NULL;
			utils::D3d::CreateRenderTargetView(m_renderTarget, m_shaderResource);

		}

		PostEffect::~PostEffect()
		{
			if (m_effectProperties)
				m_effectProperties->Release();

			m_renderTarget->Release();
			m_shaderResource->Release();
		}

		bool PostEffect::Init()
		{	
			Shader* renderToBackBufferShader = thomas::graphics::Shader::CreateShader("RenderToBackBuffer", thomas::graphics::Shader::InputLayouts::POST_EFFECT,
				"../res/thomasShaders/postEffect.hlsl", Scene::GetCurrentScene());
			
			s_renderToBackBuffer = new PostEffect("renderToBackBuffer", renderToBackBufferShader);

			s_cameraBuffer = utils::D3d::CreateDynamicBufferFromStruct(s_cameraBufferStruct, D3D11_BIND_CONSTANT_BUFFER);

			return true;
		}

		PostEffect * PostEffect::CreatePostEffect(std::string name, std::string postEffectType)
		{
			for (unsigned int i = 0; i < s_loadedEffects.size(); i++)
			{
				if (s_loadedEffects[i]->GetName() == name)
					return s_loadedEffects[i];
			}


			if (s_postEffectTypes.find(postEffectType) != s_postEffectTypes.end())
			{
				PostEffect* effectTemplate = s_postEffectTypes.find(postEffectType)->second;
				PostEffect* instancedEffect = effectTemplate->CreateInstance(name, effectTemplate->m_shader);
				s_loadedEffects.push_back(instancedEffect);
				return instancedEffect;
			}
			LOG("No Post Effect of type: " << postEffectType << " exists.");
			return NULL;
		}

		bool PostEffect::RegisterNewPostEffectType(std::string type, PostEffect * postFX)
		{
			if (s_postEffectTypes.find(type) == s_postEffectTypes.end()) //Material is not already registered.
			{
				s_postEffectTypes[type] = postFX;
				return true;
			}
			return false;
		}

		PostEffect * PostEffect::GetPostEffectByName(std::string name)
		{
			for (unsigned int i = 0; i < s_loadedEffects.size(); i++)
			{
				if (s_loadedEffects[i]->GetName() == name)
					return s_loadedEffects[i];
			}
		}

		std::vector<PostEffect*> PostEffect::GetLoadedPostEffects()
		{
			return s_loadedEffects;
		}

		std::vector<PostEffect*> PostEffect::GetPostEffectByShader(Shader * shader)
		{
			std::vector<PostEffect*> postEffects;
			for (unsigned int i = 0; i < s_loadedEffects.size(); i++)
			{
				if (s_loadedEffects[i]->GetShader() == shader)
					postEffects.push_back(s_loadedEffects[i]);
			}
			return postEffects;
		}

		void PostEffect::Render(ID3D11ShaderResourceView * prePostFXRender, ID3D11RenderTargetView * backBuffer, object::component::Camera* camera)
		{
			if (s_loadedEffects.empty())
				return;

			Clear();

			UpdateCameraBuffer(camera);

			ID3D11ShaderResourceView* prevRender = prePostFXRender;
			for (unsigned int i = 0; i < s_loadedEffects.size(); i++)
			{

				s_loadedEffects[i]->GetShader()->Bind();
				s_loadedEffects[i]->RenderPostEffect(prevRender);
				prevRender = s_loadedEffects[i]->GetShaderResource();
			}

			s_renderToBackBuffer->RenderPostEffect(prevRender, backBuffer);
		
		}

		void PostEffect::Clear()
		{
			float color[4] = { 0.3f, 0.4f, 0.3f, 1.0f };
			for (unsigned int i = 0; i < s_loadedEffects.size(); i++)
			{
				ThomasCore::GetDeviceContext()->ClearRenderTargetView(s_loadedEffects[i]->GetRenderTarget(), color);
			}
			
		}

		void PostEffect::Destroy()
		{
			s_renderToBackBuffer->m_effectProperties->Release();
			s_renderToBackBuffer->m_renderTarget->Release();
			s_renderToBackBuffer->m_shaderResource->Release();
			delete s_renderToBackBuffer;

			s_cameraBuffer->Release();
			s_quadVertexBuffer->Release();
			for (unsigned int i = 0; i < s_loadedEffects.size(); ++i)
			{
				s_loadedEffects[i]->m_effectProperties->Release();
				s_loadedEffects[i]->m_renderTarget->Release();
				s_loadedEffects[i]->m_shaderResource->Release();
			}
			
			for (auto const& effectTypes : s_postEffectTypes)
			{
				delete effectTypes.second;
			}	
			for (unsigned int i = 0; i < s_loadedEffects.size(); i++)
			{
				delete s_loadedEffects[i];
				
			}
		}

		bool PostEffect::RenderPostEffect(ID3D11ShaderResourceView* prevRender, ID3D11RenderTargetView* backBuffer)
		{
			m_shader->Bind();
			if (backBuffer) //Last fx so should be backbuffer
			{
				ThomasCore::GetDeviceContext()->OMSetRenderTargets(1, &backBuffer, NULL);
			}
			else
			{
				ThomasCore::GetDeviceContext()->OMSetRenderTargets(1, &m_renderTarget, NULL);
			}
			
			m_shader->BindTextureSampler(Texture::GetSamplerState(Texture::SamplerState::WRAP), 0);
			m_shader->BindResource(prevRender, 0);
			
			m_shader->BindBuffer(s_cameraBuffer, Shader::ResourceType::GAME_OBJECT);

			Bind();
			thomas::ThomasCore::GetDeviceContext()->Draw(4, 0);
			Unbind();
			m_shader->BindBuffer(NULL, Shader::ResourceType::GAME_OBJECT);
			m_shader->BindResource(NULL, 0);
			m_shader->Unbind();
			return true;
		}

		bool PostEffect::Bind()
		{
			m_shader->BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			m_shader->BindVertexBuffer(s_quadVertexBuffer, sizeof(float) * 5, 0);
			if(m_effectProperties)
				m_shader->BindBuffer(m_effectProperties, Shader::ResourceType::MATERIAL);

			for (unsigned int i = 0; i < m_textures.size(); i++)
			{
				m_textures[i]->Bind();

			}

			return true;
		}

		bool PostEffect::Unbind()
		{
			for (unsigned int i = 0; i < m_textures.size(); i++)
			{
				m_textures[i]->Unbind();
			}
			return true;
		}

		std::string PostEffect::GetName()
		{
			return m_name;
		}

		Shader * PostEffect::GetShader()
		{
			return m_shader;
		}

		ID3D11ShaderResourceView * PostEffect::GetShaderResource()
		{
			return m_shaderResource;
		}

		ID3D11RenderTargetView * PostEffect::GetRenderTarget()
		{
			return m_renderTarget;
		}

	}
}
