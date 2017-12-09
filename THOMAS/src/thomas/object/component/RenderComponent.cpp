#include "RenderComponent.h"
#include "../../graphics/Mesh.h"
#include "../../graphics/Material.h"
#include "../../graphics/Model.h"
#include "../GameObject.h"
#include "../../graphics/Renderer.h"
#include "../../Scene.h"
namespace thomas {
	namespace object {
		namespace component {

			RenderComponent::RenderComponent()
			{
				m_model = nullptr;
				m_bounds = nullptr;
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
					SAFE_DELETE(m_bounds);
					m_bounds = new utils::Bounds(m_gameObject->m_transform->GetPosition(), m_model->m_bounds->GetSize());
				}
					
			}

			void RenderComponent::Update()
			{
				if(m_bounds)
					m_bounds->center = m_gameObject->m_transform->GetPosition();
				std::vector<graphics::RenderPair*> setPairs;
				for (auto pair : m_renderPairs)
					if (pair->material)
						thomas::Scene::GetCurrentScene()->AddToRenderQueue(pair);
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

			graphics::Model * RenderComponent::GetModel()
			{
				return m_model;
			}
		}
	}
}