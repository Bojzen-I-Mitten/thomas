#pragma once
#define _XM_NO_INTRINSICS_ 1;
#include "Component.h"

namespace thomas
{
	namespace graphics { class Model; class Material; }
	namespace object
	{
		namespace component
		{
			class THOMAS_API RenderComponent : public Component
			{
			private:
			public:
				RenderComponent();
				void SetModel(std::string name);
				
				graphics::Model * GetModel();	
				void Update();

				void SetMaterial(graphics::Material* material);
				void SetMaterial(int meshIndex, graphics::Material* material);
				std::vector<graphics::Material*>* GetMaterials();

			public:
				graphics::Model* m_model;
			private:
				
				std::vector<graphics::Material*> m_sharedMaterials;
				std::vector<graphics::Material*> m_materials;
				//graphics::Geometry* m_geometry;
			};
		}
	}
}