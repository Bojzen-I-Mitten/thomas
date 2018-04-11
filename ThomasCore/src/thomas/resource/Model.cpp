#include "Model.h"

#include "../graphics/Mesh.h"
#include "../utils/AssimpLoader.h"
namespace thomas {
	namespace resource {

		//std::vector<Model*> Model::s_loadedModels;
		Model::Primitives Model::s_primitives;


		math::BoundingBox Model::GenerateBounds()
		{
			math::BoundingBox bounds = m_meshes[0]->m_bounds;
			
			for (int i = 1; i < m_meshes.size(); i++)
			{
				math::BoundingBox::CreateMerged(bounds, bounds, m_meshes[i]->m_bounds);
			}
			return bounds;
		}

		void Model::OnChanged()
		{
			for (unsigned int i = 0; i < m_meshes.size(); i++)
			{
				delete m_meshes[i];
			}
			m_meshes.clear();
			m_meshes = utils::AssimpLoader::LoadModel(m_path);
			m_bounds = GenerateBounds();
		}

		Model::Model(std::string path) : Resource(path)
		{
			m_meshes = utils::AssimpLoader::LoadModel(path);
			m_bounds = GenerateBounds();
		}
				

		std::vector<graphics::Mesh*> Model::GetMeshes()
		{
			return m_meshes;
		}


		Model::~Model()
		{
			for (unsigned int i = 0; i < m_meshes.size(); i++)
			{
				delete m_meshes[i];
			}
			m_meshes.clear();
		}

	}
}