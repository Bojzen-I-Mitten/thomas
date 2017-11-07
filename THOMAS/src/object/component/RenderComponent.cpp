#include "RenderComponent.h"
#include "../../graphics/Mesh.h"
#include "../../graphics/Material.h"
#include "../../graphics/Model.h"
namespace thomas {
	namespace object {
		namespace component {

			RenderComponent::RenderComponent() : Component("ModelComponent")
			{
			}

			void RenderComponent::SetModel(std::string name)
			{
				m_model = graphics::Model::GetModelByName(name);

				if (!m_model)
					LOG("ERROR: no model named \"" << name  << "\" is loaded");
			}

			graphics::Model * RenderComponent::GetModel()
			{
				return m_model;
			}
			void RenderComponent::Update()
			{
				if(m_model && m_alive)
					for (graphics::Mesh* mesh : m_model->GetMeshes())
						mesh->GetMaterial()->Update();
			}
		}
	}
}