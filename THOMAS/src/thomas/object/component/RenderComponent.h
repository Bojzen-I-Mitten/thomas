#pragma once
#define _XM_NO_INTRINSICS_ 1;
#include "Component.h"
#include "../../utils/Math.h"
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
				void SetModel(graphics::Model* model);
				
				graphics::Model * GetModel();	
				void Update();

				void SetMaterial(int meshIndex, graphics::Material* material);

				void OnDrawGizmos();

			public:
				graphics::Model* m_model;
				math::BoundingOrientedBox m_bounds;
			private:
				std::vector<graphics::RenderPair*> m_renderPairs;
				//graphics::Geometry* m_geometry;
			};
		}
	}
}