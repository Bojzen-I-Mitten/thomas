#include "Model.h"

#include "Mesh.h"
namespace thomas {
	namespace graphics {

		std::vector<Model*> Model::s_loadedModels;
		Model::Primitives Model::s_primitives;
		Model::Model(std::string name, std::vector<Mesh*> meshes)
		{
			m_name = name;
			m_meshes = meshes;
			m_bounds = GenerateBounds();
		}

		math::BoundingBox Model::GenerateBounds()
		{
			math::BoundingBox bounds = m_meshes[0]->m_bounds;
			
			for (int i = 1; i < m_meshes.size(); i++)
			{
				math::BoundingBox::CreateMerged(bounds, bounds, m_meshes[i]->m_bounds);
			}
			return bounds;
		}

		Model * Model::CreateModel(std::string name, std::vector<Mesh*> meshes)
		{
			Model* existingModel = GetModelByName(name);
			if (existingModel)
				return existingModel;
			else
			{
				Model* newModel = new Model(name, meshes);
				s_loadedModels.push_back(newModel);
				return newModel;
			}
		}

		Model * Model::GetModelByName(std::string name)
		{
			for (unsigned int i = 0; i < s_loadedModels.size(); i++)
			{
				if (s_loadedModels[i]->GetName() == name)
					return s_loadedModels[i];
			}
			return NULL;
		}

		std::vector<Model*> Model::GetLoadedModels()
		{
			return s_loadedModels;
		}


		std::string Model::GetName()
		{
			return m_name;
		}

		std::vector<Mesh*> Model::GetMeshes()
		{
			return m_meshes;
		}

		void Model::Destroy()
		{
			for (unsigned int i = 0; i < s_loadedModels.size(); ++i)
			{
				delete s_loadedModels[i];
			}
			s_loadedModels.clear();
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