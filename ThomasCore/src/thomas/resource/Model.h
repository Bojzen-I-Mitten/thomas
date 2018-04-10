#pragma once
#include "../Common.h"
#include "../utils/Math.h"
#include "Resource.h"
#include <vector>
namespace thomas {
	namespace graphics
	{
		class Material;
		class Mesh;
	}
	namespace resource {
		

		enum class PrimitiveType
		{
			Sphere,
			Capsule,
			Cylinder,
			Cube,
			Plane,
			Quad
		};

		class THOMAS_API Model : public Resource {
		private:
			math::BoundingBox GenerateBounds();
			void OnChanged();
		public:
			Model(std::string path);

			std::vector<graphics::Mesh*> GetMeshes();

			~Model();
			
		public:
			math::BoundingBox m_bounds;
		private:
			std::vector<graphics::Mesh*> m_meshes;

			static struct Primitives
			{
				Model* sphere;
				Model* capsule;
				Model* cylinder;
				Model* cube;
				Model* plane;
				Model* quad;
			} s_primitives;
		};
	}
}