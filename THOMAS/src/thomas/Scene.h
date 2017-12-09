#pragma once
/*
#include "graphics\Renderer.h"
#include "graphics\ParticleSystem.h"
#include "utils\FrustumCulling.h"

*/
#include "graphics\Shader.h"
#include "utils\Math.h"
namespace thomas
{
	namespace object 
	{ 
		namespace component 
		{ 
		class RenderComponent;
		class FrustumCullingComponent;
		class Camera;
		class Transform;
		} 
	}
	namespace graphics {
		class Material;
		class Model;
		struct RenderPair;
	}

	class THOMAS_API Scene
	{
	public:
		
		static void UnloadScene();
		void static UpdateCurrentScene();
		static void Render();

		void AddToRenderQueue(graphics::RenderPair* renderPair);
		void ClearRenderQueue();
		std::vector<graphics::RenderPair*> GetRenderQueue();

		void Render3D(object::component::Camera* camera);
		void Render2D(object::component::Camera* camera);
		//void RemoveMaterial(std::string name);

		//void RemoveShader(std::string name);
		graphics::Model* LoadModel(std::string name, std::string path, std::string type);
		//void RemoveModel(std::string name);
		template<typename T>
		static Scene* LoadScene(); //Set s_currentScene
		template<typename T>
		T * LoadObject();
		template<typename T>
		T * LoadObject(object::component::Transform* parent);
		template<typename T>
		T * LoadObject(thomas::math::Vector3 position, thomas::math::Quaternion rotation);
		template<typename T>
		T * LoadObject(thomas::math::Vector3 position, thomas::math::Quaternion rotation, object::component::Transform* parent);
		//void UnloadObject(std::string name);
		static Scene* GetCurrentScene();
		std::string GetName() { return m_name; }
		//static void SetStatPointer(PlayerStats* stats);
	private:
		std::vector<graphics::RenderPair*> m_renderQueue;
		std::vector<graphics::RenderPair*> m_lastFramesRenederQueue;
		static Scene* s_currentScene;
		static bool s_drawDebugPhysics;
		//static PlayerStats* s_stats;
	protected:
		std::string m_name;
		Scene(std::string name);
	};
	template<typename T>
	inline Scene * Scene::LoadScene()
	{
		if (s_currentScene)
			UnloadScene();
		Scene* scene = new T();
		s_currentScene = scene;
		LOG("Scene " + scene->GetName() + " set");
		//Flush inputs
		Input::Update();
		return scene;
	}
	template<typename T>
	T* Scene::LoadObject()
	{
		return object::GameObject::Instantiate<T>(this);
	}
	template<typename T>
	T* Scene::LoadObject(object::component::Transform* parent)
	{
		return object::GameObject::Instantiate<T>(parent, this);
	}
	template<typename T>
	T* Scene::LoadObject(thomas::math::Vector3 position, thomas::math::Quaternion rotation)
	{
		return object::GameObject::Instantiate<T>(position, rotation, this);
	}
	template<typename T>
	T* Scene::LoadObject(thomas::math::Vector3 position, thomas::math::Quaternion rotation, object::component::Transform* parent)
	{
		return object::GameObject::Instantiate<T>(position, rotation, parent, this);
	}
}