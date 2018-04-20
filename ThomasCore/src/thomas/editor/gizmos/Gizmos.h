#pragma once
#include "..\..\Common.h"
#include "..\..\utils\Math.h"
#include <memory>

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
				D3D_PRIMITIVE_TOPOLOGY topology;
				GizmoPasses pass;

				GizmoRenderCommand(std::vector<math::Vector3> v, math::Matrix m, math::Color c, D3D_PRIMITIVE_TOPOLOGY t, GizmoPasses p) :
					vertexData(v), matrix(m), color(c), topology(t), pass(p) {};
			};

		public:
			static void TransferGizmoCommands();
			static void RenderGizmos();
			static void ClearGizmos();
			static void Init();
			static void Destroy();

		public:
			static void DrawModel(resource::Model* model, int meshIndex, const math::Vector3 & position, const math::Quaternion & rotation, const math::Vector3 & scale);
			static void DrawModel(resource::Model* model, const math::Vector3 & position, const math::Quaternion & rotation, const math::Vector3 & scale);
			static void DrawWireModel(resource::Model* model, int meshIndex, const math::Vector3 & position, const math::Quaternion & rotation, const math::Vector3 & scale);
			static void DrawWireModel(resource::Model* model, const math::Vector3 & position, const math::Quaternion & rotation, const math::Vector3 & scale);
			static void DrawCube(const math::Vector3 & center, const math::Vector3 & size);
			static void DrawWireCube(const math::Vector3 & center, const math::Vector3 & size);
			static void DrawBoundingOrientedBox(const math::BoundingOrientedBox & obb);
			static void DrawBoundingSphere(const math::BoundingSphere & sphere);
			static void DrawRing(const math::Vector3 & origin, const math::Vector3 & majorAxis, const math::Vector3 & minorAxis);
			static void DrawLine(const math::Vector3 & from, const math::Vector3 & to);
			static void DrawSphere(const math::Vector3 & center, float radius);
			static void DrawWireSphere(const math::Vector3 & center, float radius);
			static void DrawRay(const math::Vector3 & from, const math::Vector3 & direction);
			static void DrawRay(const math::Ray & ray);
			static void DrawFrustum(const math::Vector3 & center, float fov, float maxRange, float minRange, float aspect);
			static void DrawFrustum(const math::BoundingFrustum & frustrum);

		public:
			static void SetColor(const math::Color & color);
			static void SetMatrix(const math::Matrix & matrix);

		private:
			static void DrawLines(std::vector<math::Vector3> lines);

		private:
			static std::vector<GizmoRenderCommand> s_gizmoCommands;
			static std::vector<GizmoRenderCommand> s_prevGizmoCommands;
			static std::unique_ptr<resource::Material> s_gizmoMaterial;
			static std::unique_ptr<utils::buffers::VertexBuffer> s_vertexBuffer;
			static math::Matrix s_matrix;
			static math::Color s_color;
		};
	}
}