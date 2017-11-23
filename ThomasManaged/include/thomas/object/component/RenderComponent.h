#pragma once
#define _XM_NO_INTRINSICS_ 1;
#include "Component.h"

namespace thomas
{
	namespace graphics { class Model; class Material; struct RenderPair; }
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

				void SetMaterial(int meshIndex, graphics::Material* material);

			public:
				graphics::Model* m_model;
			private:
				
				std::vector<graphics::RenderPair*> m_renderPairs;
				//graphics::Geometry* m_geometry;
			};
		}
	}
}