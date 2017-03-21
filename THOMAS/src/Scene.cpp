#include "Scene.h"
#include "graphics\PostEffect.h"
#include "graphics\Model.h"
#include "graphics\Sprite.h"
#include "graphics\Shader.h"
#include "utils\DebugTools.h"
#include "graphics\GeometryDraw.h"

namespace thomas
{
	Scene* Scene::s_currentScene;
	bool Scene::s_drawDebugPhysics;
	//PlayerStats* Scene::s_stats;

	std::vector<object::component::RenderComponent*> Scene::GetAllRenderComponents()
	{
		std::vector<object::component::RenderComponent*> renderComponents;
		for (object::GameObject* gameObject : object::GameObject::FindGameObjectsWithComponent<object::component::RenderComponent>())
		{
			if (gameObject->GetActive())
				for (object::component::RenderComponent* renderComponent : gameObject->GetComponents<object::component::RenderComponent>())
				{
					if (renderComponent->GetActive())
						renderComponents.push_back(renderComponent);
				}
		}
		return renderComponents;
	}
	void Scene::UnloadScene()
	{
		utils::DebugTools::RemoveAllVariables();

		graphics::LightManager::Destroy();
		graphics::Material::Destroy();
		graphics::Shader::Destroy(s_currentScene);
		graphics::Texture::Destroy();
		graphics::Model::Destroy();
		
		Physics::Destroy();
		//graphics::Sprite::Destroy();
		//graphics::TextRender::Destroy();
		object::Object::Destroy(s_currentScene);
		
		object::GameObject::Destroy();
		//object::Object::Destroy();
		//graphics::ParticleSystem::Destroy();
		//graphics::ParticleSystem::Init();
		
		
		delete s_currentScene;
		s_currentScene = nullptr;
		ThomasTime::SetTimescale(1.0f);
		object::Object::Clean();
		graphics::GeometryDraw::Destroy();
	}
	void Scene::UpdateCurrentScene()
	{
		//Temp fix for ocean.
		graphics::Renderer::RenderSetup(NULL);
		if (s_currentScene)
		{
			std::string name = s_currentScene->GetName();
			for (object::Object* object : object::Object::GetAllObjectsInScene(s_currentScene))
			{
				if (name != s_currentScene->GetName())
					break;
				if (object->GetActive())
					object->Update();
				
			}
			for (object::Object* object : object::Object::GetAllObjectsInScene(s_currentScene))
			{
				if (name != s_currentScene->GetName())
					break;
				if (object->GetActive())
					object->LateUpdate();
			}

		}
		else
			LOG("No scene set");
		graphics::Renderer::ResetDepthStencilState();
		object::Object::Clean();
	}
	void Scene::Render()
	{
		if (s_currentScene == NULL)
		{
			LOG("No scene set")
				return;
		}
		std::vector<object::GameObject*> cameraObjects = object::GameObject::FindGameObjectsWithComponent<object::component::Camera>();
		std::vector<object::component::Camera*> cameras;
		for (object::GameObject* object : cameraObjects)
			cameras.push_back(object->GetComponent<object::component::Camera>());
		for (object::component::Camera* camera : cameras)
		{
			graphics::Renderer::Clear();
			graphics::Renderer::RenderSetup(camera);

			s_currentScene->Render3D(camera);
			if (s_drawDebugPhysics)
				Physics::DrawDebug(camera);
			s_currentScene->Render2D(camera);
			graphics::GeometryDraw::Draw();

			graphics::PostEffect::Render(graphics::Renderer::GetDepthBufferSRV(), graphics::Renderer::GetBackBuffer(), camera);

			utils::DebugTools::Draw();
			ThomasCore::GetSwapChain()->Present(0, 0);
		}
	}
	void Scene::Render3D(object::component::Camera * camera)
	{
		std::vector<object::component::RenderComponent*> renderComponents = GetAllRenderComponents();

		utils::FrustumCulling::GenerateClippingPlanes(camera);

		for (graphics::Shader* shader : graphics::Shader::GetShadersByScene(s_currentScene))
		{
			shader->Bind();
			camera->BindReflection();
			graphics::Renderer::BindGameObjectBuffer();
			graphics::LightManager::BindAllLights();
			for (graphics::Material* material : graphics::Material::GetMaterialsByShader(shader))
			{
				bool matBound = false;
				for (object::component::RenderComponent* renderComponent : renderComponents)
				{
					if (renderComponent->GetModel())
					{
						object::component::FrustumCullingComponent* frustumCullingComponent = renderComponent->m_gameObject->GetComponent<object::component::FrustumCullingComponent>();
						if (frustumCullingComponent == NULL || utils::FrustumCulling::Cull(frustumCullingComponent))
						{
							std::vector<graphics::Mesh*> meshes = renderComponent->GetModel()->GetMeshesByMaterial(material);
							if (!meshes.empty())
							{
								if (!matBound)
								{
									material->Bind();
									matBound = true;
								}
								graphics::Renderer::UpdateGameObjectBuffer(camera, renderComponent->m_gameObject);
								for (graphics::Mesh* mesh : meshes)
								{
									mesh->Bind();
									mesh->Draw();
									mesh->Unbind();
								}
							}
						}
					}
				}
				material->Unbind();
			}
			shader->Unbind();
		}
		
		camera->BindSkybox();
		camera->UnbindSkybox();


		ThomasCore::GetDeviceContext()->OMSetDepthStencilState(graphics::ParticleSystem::GetDepthStencilState(), 1);
		for (object::GameObject* gameObject : object::GameObject::FindGameObjectsWithComponent<object::component::ParticleEmitterComponent>())
		{
			if (gameObject->GetActive())
				for (object::component::ParticleEmitterComponent* emitterComponent : gameObject->GetComponents<object::component::ParticleEmitterComponent>())
				{
					if (emitterComponent->GetActive() && emitterComponent->GetSpawnedParticleCount() > 0 && emitterComponent->GetDrawTimer() > 0)
						graphics::ParticleSystem::DrawParticles(camera, emitterComponent);
				}
		}

		graphics::Renderer::ResetDepthStencilState();
	}



	void Scene::Render2D(object::component::Camera * camera)
	{


		for (object::GameObject* gameObject : object::GameObject::FindGameObjectsWithComponent<object::component::SpriteComponent>())
		{
			if (gameObject->GetActive())
				for (object::component::SpriteComponent* sprite : gameObject->GetComponents<object::component::SpriteComponent>())
				{
					if (sprite->GetActive())
						graphics::Sprite::RenderImage(sprite);
				}
		}

		for (object::GameObject* gameObject : object::GameObject::FindGameObjectsWithComponent<object::component::TextComponent>())
		{
			if (gameObject->GetActive())
				for (object::component::TextComponent* text : gameObject->GetComponents<object::component::TextComponent>())
				{
					if (text->GetActive())
						graphics::TextRender::RenderText(text);
				}
		}
	}


	graphics::Material * Scene::LoadMaterial(std::string type, graphics::Material* material)
	{
		return graphics::Material::RegisterNewMaterialType(type, material);
	}
	graphics::Shader * Scene::LoadShader(std::string name, thomas::graphics::Shader::InputLayouts inputLayout, std::string path)
	{
		return thomas::graphics::Shader::CreateShader(name, inputLayout, path, this);
	}
	graphics::Shader * Scene::LoadShader(std::string name, thomas::graphics::Shader::InputLayouts inputLayout, std::string vertexShader, std::string geometryShader, std::string hullShader, std::string domainShader, std::string pixelShader)
	{
		return thomas::graphics::Shader::CreateShader(name, inputLayout, vertexShader, geometryShader, hullShader, domainShader, pixelShader, this);
	}
	graphics::Model * Scene::LoadModel(std::string name, std::string path, std::string type)
	{
		return utils::AssimpLoader::LoadModel(name, path, type);
	}
	Scene * Scene::GetCurrentScene()
	{
		if (s_currentScene)
			return s_currentScene;
		LOG("No scene set")
			return NULL;
	}
	/*void Scene::SetStatPointer(PlayerStats* stats)
	{
		s_stats = stats;
	}*/
	Scene::Scene(std::string name)
	{
		m_name = name;
		s_drawDebugPhysics = false;
		utils::DebugTools::AddBool(s_drawDebugPhysics, "Draw Debug Physics");
	}
}
