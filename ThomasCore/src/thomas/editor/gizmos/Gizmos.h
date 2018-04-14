#pragma once
#include "../../Common.h"
#include "../../utils/Math.h"
namespace thomas
{
	namespace resource { class Model; class Material; }
	namespace utils { struct Ray; namespace buffers { class VertexBuffer; } }
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

			struct GizmoRenderCommand
			{
				math::Matrix matrix;
				std::vector<math::Vector3> vertexData;
				math::Color color;
				//Add texture
				D3D_PRIMITIVE_TOPOLOGY topology;
				GizmoPasses pass;

				GizmoRenderCommand(std::vector<math::Vector3> v, math::Matrix m, math::Color c, D3D_PRIMITIVE_TOPOLOGY t, GizmoPasses p) :
					vertexData(v), matrix(m), color(c), topology(t), pass(p) {};
			};
			static void DrawLines(std::vector<math::Vector3> lines);
		public:
			static void TransferGizmoCommands();
			static void RenderGizmos();
			static void ClearGizmos();
			static void Init();
			static void Destroy();
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
			static void DrawBillboard(math::Vector3 centerPos, float width, float height);

			static void DrawFrustum(math::Vector3 center, float fov, float maxRange, float minRange, float aspect);
			static void DrawFrustum(math::BoundingFrustum& frustrum);

			static void SetColor(math::Color color);
			static void SetMatrix(math::Matrix matrix);
		private:
			static std::vector<GizmoRenderCommand> s_gizmoCommands;
			static std::vector<GizmoRenderCommand> s_prevGizmoCommands;
			static resource::Material* s_gizmoMaterial;
			static utils::buffers::VertexBuffer* s_vertexBuffer;
			static math::Matrix s_matrix;
			static math::Color s_color;
		};
	}
}