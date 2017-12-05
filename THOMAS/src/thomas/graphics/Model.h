#pragma once
#include "../Common.h"
#include <vector>
namespace thomas {
	namespace graphics {
		class Material;
		class Mesh;

		class THOMAS_API Model {
		private:
			Model(std::string name, std::vector<Mesh*> meshes);
		public:
			static Model* CreateModel(std::string name, std::vector<Mesh*> meshes);
			static Model* GetModelByName(std::string name);
			static std::vector<Model*> GetLoadedModels();
			std::string GetName();
			std::vector<Mesh*> GetMeshes();

			static void Destroy();

			~Model();
			
		private:
			static std::vector<Model*> s_loadedModels;
			std::vector<Mesh*> m_meshes;
			std::string m_name;
		};
	}
}