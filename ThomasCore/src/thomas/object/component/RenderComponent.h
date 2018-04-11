#pragma once
#define _XM_NO_INTRINSICS_ 1;
#include "Component.h"
#include "../../utils/Math.h"
namespace thomas
{
	namespace graphics {struct RenderPair; }
	namespace resource { class Model; class Material; }
	namespace object
	{
		namespace component
		{
			class THOMAS_API RenderComponent : public Component
			{
			private:
			public:
				RenderComponent();
				void SetModel(resource::Model* model);
				
				resource::Model * GetModel();
				void Update();

				void SetMaterial(int meshIndex, resource::Material* material);

				void OnDrawGizmos();

			public:
				resource::Model* m_model;
				math::BoundingOrientedBox m_bounds;
			private:
				std::vector<resource::Material*> m_materials;
				//graphics::Geometry* m_geometry;
			};
		}
	}
}