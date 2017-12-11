#pragma once
#include "../../utils/Math.h"
#include "../../Common.h"
#include <DirectXTK\GeometricPrimitive.h>
#include <memory>
namespace thomas
{
	namespace graphics { class Material; class Model; }
	namespace editor
	{
		class THOMAS_API GizmoRender
		{
		private:
			struct LineVertex
			{
				math::Vector3 pos;
			};
			static graphics::Material* s_material;
			static graphics::Model* s_cone;
		public:
			static void Init();
			static void DrawAxis(const math::Vector3& origin, const math::Vector3& axis, const math::Vector3& perp, const math::Color& color);
		};
	}
}