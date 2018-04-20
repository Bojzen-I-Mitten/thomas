#pragma once

#include <map>
#include <memory>
namespace thomas
{
	namespace resource
	{
		class Model;
	}
	namespace utils
	{
		class Primitives
		{
		public:
			enum class Type
			{
				Sphere,
				Capsule,
				Cylinder,
				Cube,
				Plane,
				Quad
			};

			static void Init();
			static void Destroy();

			static resource::Model* GetPrimitive(Type type);

		private:
			static std::map<Type, resource::Model*> s_primitives;
		};
	}
}