#include "Object.h"
#include "..\Scene.h"


namespace thomas
{
	namespace object
	{
		std::vector<Object*> Object::s_objects;

		Object::~Object()
		{
		}

		std::string Object::GetName()
		{
			return m_name;
		}
		Scene* Object::GetScene()
		{
			return m_scene;
		}
		bool Object::Destroy(Object *object)
		{
			object->m_alive = false;
			object->OnDestroy();
			return true;
		}
		void Object::Destroy(Scene * scene)
		{
			for (int i = 0; i < s_objects.size(); ++i)
			{
				if (s_objects[i]->m_scene == scene)
					Destroy(s_objects[i]);
			}
				
		}

		

		void Object::Destroy()//this should be static right
		{
			for (int i = 0; i < s_objects.size(); ++i)
			{
				Destroy(s_objects[i]);
			}
			Clean();
			s_objects.clear();
			s_objects.resize(0);
		}
		bool Object::IsAlive(const Object* object)
		{
			return object->m_alive;
		}
		std::vector<Object*> Object::GetObjects()
		{
			return s_objects;
		}
		std::vector<Object*> Object::GetAllObjectsInScene(Scene * scene)
		{
			std::vector<Object*> output;
			for (Object* object : s_objects)
				if (object->m_scene == scene && object->m_alive)
					output.push_back(object);
			return output;
		}
		void Object::Clean()
		{
			for (int i =0;i<s_objects.size();i++)
			{
				if (s_objects[i]->m_alive == false)
				{
					if(s_objects[i])
						delete s_objects[i];
					s_objects.erase(s_objects.begin() + i);
					i -= 1;
				}
					
			}
		}
	}
}

