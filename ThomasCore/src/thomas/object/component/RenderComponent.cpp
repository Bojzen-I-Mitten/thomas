#include "RenderComponent.h"
#include "../../graphics/Mesh.h"
#include "../../resource/Material.h"
#include "../../resource/Model.h"
#include "../GameObject.h"
#include "../../graphics/Renderer.h"
#include "../../editor/gizmos/Gizmos.h"

namespace thomas {
	namespace object {
		namespace component {

			std::vector<RenderComponent*> RenderComponent::s_renderComponents;
			RenderComponent::RenderComponent()
			{
				m_model = nullptr;
				m_bounds = math::BoundingOrientedBox();
				m_bounds.Extents.x = 0;
				m_bounds.Extents.y = 0;
				m_bounds.Extents.z = 0;
				s_renderComponents.push_back(this);
			}
			void RenderComponent::SetModel(resource::Model* model)
			{
				if (!model)
				{
					m_model = model;
					m_bounds = math::BoundingOrientedBox();
					m_bounds.Extents.x = 0;
					m_bounds.Extents.y = 0;
					m_bounds.Extents.z = 0;
				}
				else
				{
					m_model = model;
					
				}
			}

			void RenderComponent::Update()
			{
				if (m_model)
				{
					math::BoundingOrientedBox::CreateFromBoundingBox(m_bounds, m_model->m_bounds);
					m_bounds.Transform(m_bounds, m_gameObject->m_transform->GetWorldMatrix());

					if (m_model->GetMeshes().size() != m_materials.size())
						m_materials.resize(m_model->GetMeshes().size());
				}		
			}
			void RenderComponent::SetMaterial(int meshIndex, resource::Material * material)
			{
				if (!material)
				{
					LOG("Material is NULL");
					return;
				}
				if (meshIndex < m_model->GetMeshes().size() && meshIndex >= 0)
				{
					m_materials[meshIndex] = material;
				}
					
			}


			void RenderComponent::SubmitToRenderer(Camera* camera)
			{
				if (m_model)
				{
					for (int i = 0; i < m_model->GetMeshes().size(); i++)
					{
						resource::Material* material = m_materials[i];
						if (material == nullptr)
							material = resource::Material::GetStandardMaterial();

						graphics::Mesh* mesh = m_model->GetMeshes()[i];
						
						thomas::graphics::Renderer::SubmitCommand(thomas::graphics::RenderCommand(m_gameObject->m_transform->GetWorldMatrix(), mesh, material, camera));
					}
				}
				
			}

			void RenderComponent::OnDestroy()
			{
				for (int i = 0; i < s_renderComponents.size(); i++)
				{
					if (s_renderComponents[i] == this)
					{
						s_renderComponents.erase(s_renderComponents.begin() + i);
						return;
					}
				}
			}

			std::vector<RenderComponent*> RenderComponent::GetAllRenderComponents()
			{
				return s_renderComponents;
			}

			void RenderComponent::OnDrawGizmos()
			{/*
				editor::Gizmos::SetMatrix(math::Matrix::CreateWorld(m_bounds.Center, math::Vector3::Forward, math::Vector3::Up));
				editor::Gizmos::SetColor(math::Color(0, 0, 1));
				editor::Gizmos::DrawBoundingOrientedBox(m_bounds);*/
			}

			resource::Model * RenderComponent::GetModel()
			{
				return m_model;
			}
		}
	}
}