#pragma once
#include "../../Common.h"
#include "../../utils/Math.h"
namespace thomas
{
	namespace resource { class Model; class Material; }
	namespace utils { struct Ray; }
	namespace editor
	{
		class THOMAS_API Gizmos
		{

		private:
			enum class GizmoPasses
			{
				SOLID = 0,
				WIREFRAME = 1,
			};

		public:

			static void Init();

			static void DrawModel(resource::Model* model, int meshIndex, math::Vector3 position, math::Quaternion rotation, math::Vector3 scale);
			static void DrawModel(resource::Model* model, math::Vector3 position, math::Quaternion rotation, math::Vector3 scale);

			static void DrawWireModel(resource::Model* model, int meshIndex, math::Vector3 position, math::Quaternion rotation, math::Vector3 scale);
			static void DrawWireModel(resource::Model* model, math::Vector3 position, math::Quaternion rotation, math::Vector3 scale);

			static void DrawCube(math::Vector3 center, math::Vector3 size);
			static void DrawWireCube(math::Vector3 center, math::Vector3 size);
			static void DrawBoundingOrientedBox(math::BoundingOrientedBox& obb);
			static void DrawBoundingSphere(math::BoundingSphere& sphere);
			static void DrawRing(math::Vector3 origin, math::Vector3 majorAxis, math::Vector3 minorAxis);
			static void DrawLine(math::Vector3 from, math::Vector3 to);
			static void DrawSphere(math::Vector3 center, float radius);
			static void DrawWireSphere(math::Vector3 center, float radius);
			static void DrawRay(math::Vector3 from, math::Vector3 direction);
			static void DrawRay(math::Ray ray);

			static void DrawFrustum(math::Vector3 center, float fov, float maxRange, float minRange, float aspect);
			static void DrawFrustum(math::BoundingFrustum& frustrum);

			static void SetColor(math::Color color);
			static void SetMatrix(math::Matrix matrix);
		private:
			static resource::Material* s_gizmoMaterial;
		};
	}
}