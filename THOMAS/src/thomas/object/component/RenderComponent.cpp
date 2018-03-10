#include "RenderComponent.h"
#include "../../graphics/Mesh.h"
#include "../../graphics/Material.h"
#include "../../graphics/Model.h"
#include "../GameObject.h"
#include "../../graphics/Renderer.h"
#include "../../editor/gizmos/Gizmos.h"

namespace thomas {
	namespace object {
		namespace component {

			RenderComponent::RenderComponent()
			{
				m_model = nullptr;
				m_bounds = math::BoundingOrientedBox();
			}

			void RenderComponent::SetModel(graphics::Model* model)
			{
				if (!model)
				{
					LOG("ERROR: model was NULL");
				}
				else
				{
					for (graphics::RenderPair* pair : m_renderPairs)
						delete pair;
					m_renderPairs.clear();
					std::vector<graphics::Mesh*> meshes = model->GetMeshes();
					for (graphics::Mesh* mesh : meshes)
					{
						graphics::RenderPair* renderPair = new graphics::RenderPair();
						renderPair->mesh = mesh;
						renderPair->material = graphics::Material::GetStandardMaterial();
						renderPair->transform = m_gameObject->m_transform;
						
						m_renderPairs.push_back(renderPair);
					}
					m_model = model;
					
				}
					
			}

			void RenderComponent::Update()
			{
				if (m_model)
				{
					math::BoundingOrientedBox::CreateFromBoundingBox(m_bounds, m_model->m_bounds);
					m_bounds.Transform(m_bounds, m_gameObject->m_transform->GetWorldMatrix());
				}
				
				std::vector<graphics::RenderPair*> setPairs;
				for (auto pair : m_renderPairs)
					if (pair->material)
						thomas::graphics::Renderer::AddToRenderQueue(pair);
			}
			void RenderComponent::SetMaterial(int meshIndex, graphics::Material * material)
			{
				if (!material)
				{
					LOG("Material is NULL");
					return;
				}
				if (meshIndex < m_renderPairs.size())
					m_renderPairs[meshIndex]->material = material;
			}

			void RenderComponent::OnDrawGizmos()
			{
				editor::Gizmos::SetMatrix(math::Matrix::Identity);
				editor::Gizmos::SetColor(math::Color(0, 0, 1));
				editor::Gizmos::DrawBoundingOrientedBox(m_bounds);
			}

			graphics::Model * RenderComponent::GetModel()
			{
				return m_model;
			}
		}
	}
}