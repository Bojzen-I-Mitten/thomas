#pragma once
#define _XM_NO_INTRINSICS_ 1;
#include "Component.h"
#include "../../graphics/Model.h"
namespace thomas
{
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
			private:
				graphics::Model* m_model;
				//graphics::Geometry* m_geometry;
			};
		}
	}
}