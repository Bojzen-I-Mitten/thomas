#include "Primitives.h"
#include "../resource/Model.h"
namespace thomas
{
	namespace utils
	{
		std::map<Primitives::Type, resource::Model*> Primitives::s_primitives;

		void Primitives::Init()
		{
			s_primitives[Type::Cube] = new resource::Model("../Data/primitives/cube.obj");
			s_primitives[Type::Plane] = new resource::Model("../Data/primitives/plane.obj");
			s_primitives[Type::Cylinder] = new resource::Model("../Data/primitives/cylinder.obj");
			s_primitives[Type::Quad] = new resource::Model("../Data/primitives/quad.obj");
			s_primitives[Type::Sphere] = new resource::Model("../Data/primitives/sphere.obj");
			s_primitives[Type::Capsule] = new resource::Model("../Data/primitives/capsule.obj");
		}

		void Primitives::Destroy()
		{
			for (auto& primitive : s_primitives)
				delete primitive.second;
			s_primitives.clear();
		}

		resource::Model* Primitives::GetPrimitive(Type type)
		{
			return s_primitives[type];
		}

	}
}
