#pragma once
#include "../Common.h"
#include "../utils/Math.h"
#include <vector>
namespace thomas {
	namespace graphics {
		class Material;
		class Mesh;

		enum class PrimitiveType
		{
			Sphere,
			Capsule,
			Cylinder,
			Cube,
			Plane,
			Quad
		};

		class THOMAS_API Model {
		private:
			Model(std::string name, std::vector<Mesh*> meshes);
			math::BoundingBox GenerateBounds();
		public:
			static Model* CreateModel(std::string name, std::vector<Mesh*> meshes);
			static Model* GetModelByName(std::string name);
			static std::vector<Model*> GetLoadedModels();
			std::string GetName();
			std::vector<Mesh*> GetMeshes();

			static void Destroy();

			~Model();
			
		public:
			math::BoundingBox m_bounds;
		private:
			static std::vector<Model*> s_loadedModels;
			std::vector<Mesh*> m_meshes;
			std::string m_name;

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