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

			RenderComponent::RenderComponent() : Component("RenderComponent")
			{
			}

			void RenderComponent::SetModel(std::string name)
			{

				graphics::Model* model = graphics::Model::GetModelByName(name);				
				if (!model)
				{
					LOG("ERROR: no model named \"" << name << "\" is loaded");
				}
				else
				{
					m_renderPairs.clear();
					std::vector<graphics::Mesh*> meshes = model->GetMeshes();
					for (graphics::Mesh* mesh : meshes)
					{
						graphics::RenderPair* renderPair = new graphics::RenderPair();
						renderPair->mesh = mesh;
						renderPair->material = NULL;
						renderPair->transform = m_gameObject->m_transform;

						m_renderPairs.push_back(renderPair);
					}
				}
					
			}

			void RenderComponent::Update()
			{
				std::vector<graphics::RenderPair*> setPairs;
				for (auto pair : m_renderPairs)
					if (pair->material)
						m_scene->AddToRenderQueue(pair);
			}

			void RenderComponent::SetMaterial(int meshIndex, graphics::Material * material)
			{
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