#include "RenderComponent.h"
#include "../../graphics/Mesh.h"
#include "../../graphics/Material.h"
#include "../../graphics/Model.h"
#include "../GameObject.h"
namespace thomas {
	namespace object {
		namespace component {

			RenderComponent::RenderComponent() : Component("RenderComponent")
			{
			}

			void RenderComponent::SetModel(std::string name)
			{
				m_model = graphics::Model::GetModelByName(name);
				m_materials.clear();
				
				if (!m_model)
				{
					LOG("ERROR: no model named \"" << name << "\" is loaded");
				}
				else
				{
					m_materials.resize(m_model->GetMeshes().size());
				}
					
			}

			void RenderComponent::SetMaterial(graphics::Material * material)
			{
				if (m_model->GetMeshes().size() > 0)
				{
					m_materials[0] = material;
				}
					
			}

			void RenderComponent::SetMaterial(int meshIndex, graphics::Material * material)
			{
				if (meshIndex < m_model->GetMeshes().size())
					m_materials[meshIndex] = material;
			}

			graphics::Model * RenderComponent::GetModel()
			{
				return m_model;
			}
			std::vector<graphics::Material*>* RenderComponent::GetMaterials()
			{
				return &m_materials;
			}

			void RenderComponent::Update()
			{
				for (graphics::Material* material : m_materials)
				{
					if (material != nullptr)
					{
						material->SetMatrix("worldMatrix", m_gameObject->GetComponent<Transform>()->GetWorldMatrix());
					}
				}
				
			}
		}
	}
}