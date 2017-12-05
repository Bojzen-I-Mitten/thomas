#pragma once
#include "../Common.h"
#include <string>
#include <vector>
namespace thomas
{
	class Scene;
	namespace object
	{
		class THOMAS_API Object
		{
		private:
		public:
			virtual ~Object();

			virtual void OnEnable(){}
			virtual void OnDisable(){}
			virtual void OnDestroy() {}
			virtual void Start() {}
			virtual void Update() {}
			virtual void FixedUpdate() {}
			virtual void LateUpdate() {}
			virtual void Render() {}
			std::string GetName();
			Scene* GetScene();

			static bool Destroy(Object *object);
			static void Destroy(Scene* scene);
			static void Destroy();
			static bool IsAlive(const Object* object);
			bool Alive() { return m_alive; }
			//Clone object
			template<typename T>
			static T* Instantiate(Scene* scene);

			static std::vector<Object*> GetObjects();
			static Object* GetObjectByType(std::string type);
			
			template<typename T>
			static std::vector<T*> FindObjectsOfType();

			static std::vector<Object*> GetAllObjectsInScene(Scene* scene);

			static void Clean();

		private:
			bool m_stateSet;
			static std::vector<Object*> s_objects;
		protected:
			bool m_active;
			bool m_alive;
			std::string m_name;
			Scene* m_scene;
		};
		template<typename T>
		inline T * Object::Instantiate(Scene * scene)
		{
			T* object = new T();
			s_objects.push_back(object);
			object->m_scene = scene;
			return object;
		}
		template<typename T>
		inline std::vector<T*> Object::FindObjectsOfType()
		{
			std::vector<T*> objects;
			std::vector<GameObject*> gameObjectsWithComponent;
			for (UINT i = 0; i < s_objects.size(); i++)
			{
				T* object = dynamic_cast<T*>(s_objects[i]);
				if (object)
					objects.push_back(object);
			}
			return objects;
		}
	}
}