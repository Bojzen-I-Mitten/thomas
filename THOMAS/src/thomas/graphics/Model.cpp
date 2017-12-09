#include "Model.h"

#include "Mesh.h"
namespace thomas {
	namespace graphics {

		std::vector<Model*> Model::s_loadedModels;
		Model::Model(std::string name, std::vector<Mesh*> meshes)
		{
			m_name = name;
			m_meshes = meshes;
			m_bounds = GenerateBounds();
		}

		utils::Bounds * Model::GenerateBounds()
		{
			if (m_meshes.empty())
				return new utils::Bounds(math::Vector3(), math::Vector3());
			utils::Bounds* meshBound = m_meshes[0]->m_bounds;
			math::Vector3 minVector = meshBound->GetMin();
			math::Vector3 maxVector = meshBound->GetMax();

			for (int i = 1; i < m_meshes.size(); i++)
			{
				meshBound = m_meshes[i]->m_bounds;
				math::Vector3 meshMin = meshBound->GetMin();
				math::Vector3 meshMax = meshBound->GetMax();

				if (minVector.x > meshMin.x)
					minVector.x = meshMin.x;
				if (minVector.y > meshMin.y)
					minVector.y = meshMin.y;
				if (minVector.z > meshMin.z)
					minVector.z = meshMin.z;

				if (maxVector.x > meshMax.x)
					maxVector.x = meshMax.x;
				if (maxVector.y > meshMax.y)
					maxVector.y = meshMax.y;
				if (maxVector.z > meshMax.z)
					maxVector.z = meshMax.z;
			}

			math::Vector3 size = math::Vector3(abs(minVector.x - maxVector.x), abs(minVector.y - maxVector.y), abs(minVector.z - maxVector.z));
			return new utils::Bounds(math::Vector3(), size);
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
			delete m_bounds;
		}

	}
}