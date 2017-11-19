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
			Object(std::string type);
			virtual ~Object();

			virtual void OnEnable(){}
			virtual void OnDisable(){}
			virtual void OnDestroy() {}
			virtual void Start() {}
			virtual void Update() {}
			virtual void FixedUpdate() {}
			virtual void LateUpdate() {}
			virtual void Render() {}
			virtual void SetActive(bool active);
			virtual bool GetActive() { return m_active; }
			std::string GetType();
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

			static std::vector<Object*> GetAllObjectsInScene(Scene* scene);

			static void Clean();

		private:
			bool m_stateSet;
			static std::vector<Object*> s_objects;
		protected:
			bool m_active;
			bool m_alive;
			std::string m_type;
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
	}
}