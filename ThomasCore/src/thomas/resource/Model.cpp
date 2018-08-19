#include "Model.h"

#include "../graphics/Mesh.h"
#include "../utils/AssimpLoader.h"
namespace thomas {
	namespace resource {

		math::BoundingBox Model::GenerateBounds()
		{
			if (m_data.meshes.size() == 0)
				return math::BoundingBox();
			math::BoundingBox bounds = m_data.meshes[0]->m_bounds;
			
			for (int i = 1; i < m_data.meshes.size(); i++)
			{
				math::BoundingBox::CreateMerged(bounds, bounds, m_data.meshes[i]->m_bounds);
			}
			return bounds;
		}

		void Model::OnChanged()
		{
			m_data.meshes.clear();
			//m_data.meshes = utils::AssimpLoader::LoadModel(m_path);
			m_bounds = GenerateBounds();
		}

		Model::Model(std::string path) : Resource(path)
		{
			m_data = utils::AssimpLoader::LoadModel(path);
			m_bounds = GenerateBounds();
		}
		std::vector<std::shared_ptr<graphics::Mesh>> Model::GetMeshes()
		{
			return m_data.meshes;
		}

		std::vector<Model::BoneInfo> Model::GetBones()
		{
			return m_data.boneInfo;
		}


		Model::~Model()
		{
			m_data.meshes.clear();
		}

	}
}