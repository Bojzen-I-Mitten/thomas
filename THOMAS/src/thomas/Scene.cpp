#include "Scene.h"
#include "graphics\Model.h"
#include "graphics\Sprite.h"
#include "graphics\Shader.h"
#include "utils\DebugTools.h"
#include "utils\AssimpLoader.h"
#include "object\GameObject.h"
#include "graphics\Material.h"
#include "graphics\Renderer.h"
#include "utils\FrustumCulling.h"
#include "graphics\Mesh.h"
#include "graphics\Texture.h"
#include "graphics\ParticleSystem.h"
#include "graphics\TextRender.h"
#include "object\component\RenderComponent.h"
#include "graphics\LightManager.h"
#include "object\component\SpriteComponent.h"
#include "object\component\ParticleEmitterComponent.h"
#include "object\component\TextComponent.h"
#include "ThomasTime.h"
#include "ThomasCore.h"
#include "object\component\Camera.h"
#include "graphics\Material.h"
#include "editor\EditorCamera.h"
#include "Window.h"
namespace thomas
{
	Scene* Scene::s_currentScene;
	bool Scene::s_drawDebugPhysics;
	//PlayerStats* Scene::s_stats;

	
	void Scene::UnloadScene()
	{
		utils::DebugTools::RemoveAllVariables();

		graphics::LightManager::Destroy();
		graphics::Texture::Destroy();
		graphics::Model::Destroy();
		
		Physics::Destroy();
		//graphics::Sprite::Destroy();
		//graphics::TextRender::Destroy();

		object::GameObject::Destroy();
		object::Object::Destroy();
		//object::Object::Destroy();
		//graphics::ParticleSystem::Destroy();
		//graphics::ParticleSystem::Init();
		
		
		delete s_currentScene;
		s_currentScene = nullptr;
		ThomasTime::SetTimescale(1.0f);
	}
	void Scene::UpdateCurrentScene()
	{
		if (s_currentScene)
		{
			object::Object::Clean();
			editor::EditorCamera::Update();
			/*for (object::GameObject* gameObject : object::GameObject::GetGameObjects())
			{
				for (object::component::Component* component : gameObject->m_components)
					component->Update();
			}*/
		}
			
	}
	void Scene::Render()
	{
		if (s_currentScene == NULL)
		{
			LOG("No scene set")
				return;
		}

		if (Window::GetEditorWindow() && Window::GetEditorWindow()->Initialized())
		{
			Window::ClearAllWindows();

			graphics::Renderer::Begin();
			//Editor rendering
			editor::EditorCamera::Render();
			//end editor rendering

			for (object::component::Camera* camera : object::component::Camera::s_allCameras)
			{
				camera->Render();
			}
			Window::PresentAllWindows();
		}

	}
	void Scene::AddToRenderQueue(graphics::RenderPair * renderPair)
	{
		m_renderQueue.push_back(renderPair);
	}
	void Scene::ClearRenderQueue()
	{
		if(s_currentScene)
			s_currentScene->m_renderQueue.clear();
	}
	std::vector<graphics::RenderPair*> Scene::GetRenderQueue()
	{
		return m_renderQueue;
	}


	void Scene::Render2D(object::component::Camera * camera)
	{


		/*for (object::GameObject* gameObject : object::GameObject::FindGameObjectsWithComponent<object::component::SpriteComponent>())
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
		}*/
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
		//utils::DebugTools::AddBool(s_drawDebugPhysics, "Draw Debug Physics");
	}
}
