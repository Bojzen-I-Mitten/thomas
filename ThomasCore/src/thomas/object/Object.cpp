#include "Object.h"


namespace thomas
{
	namespace object
	{
		std::vector<Object*> Object::s_objects;

		void Object::Add(Object* object)
		{
			s_objects.push_back(object);
		}

		void Object::Clean()
		{
			for (int i = 0; i < s_objects.size(); i++)
			{
				if (s_objects[i]->m_destroyed)
				{
					delete s_objects[i];
					s_objects.erase(s_objects.begin() + i);
					--i;
				}
			}
		}

		Object::~Object()
		{
		}

		std::string Object::GetName()
		{
			return m_name;
		}
		void Object::SetName(std::string name)
		{
			m_name = name;
		}
		bool Object::Destroy(Object *object)
		{
			if (!object->m_destroyed)
			{
				object->OnDestroy();
				object->m_destroyed = true;
			}
			return true;
		}


		void Object::Destroy()//this should be static right
		{
			for (int i = 0; i < s_objects.size(); ++i)
			{
				Destroy(s_objects[i]);
			}
			s_objects.clear();
			s_objects.resize(0);
		}

		std::vector<Object*> Object::GetObjects()
		{
			return s_objects;
		}


	}
}

