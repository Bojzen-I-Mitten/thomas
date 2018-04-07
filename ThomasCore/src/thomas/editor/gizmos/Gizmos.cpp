#include "Gizmos.h"
#include "../../resource/Model.h"
#include "../../graphics/Material.h"
#include "../../graphics/Shader.h"
#include "../../graphics/Mesh.h"
#include "../../utils/d3d.h"
namespace thomas
{
	namespace editor
	{
		graphics::Material* Gizmos::s_gizmoMaterial;
		void Gizmos::DrawModel(resource::Model * model, math::Vector3 position = math::Vector3::Zero, math::Quaternion rotation = math::Quaternion::Identity, math::Vector3 scale = math::Vector3::One)
		{
			DrawModel(model, -1, position, rotation, scale);
		}


		void Gizmos::DrawModel(resource::Model * model, int meshIndex, math::Vector3 position = math::Vector3::Zero, math::Quaternion rotation = math::Quaternion::Identity, math::Vector3 scale = math::Vector3::One)
		{
			
			s_gizmoMaterial->SetShaderPass((int)GizmoPasses::SOLID);
			s_gizmoMaterial->m_topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			s_gizmoMaterial->SetMatrix("gizmoMatrix", math::CreateMatrix(position, rotation, scale));
			s_gizmoMaterial->Bind();
			std::vector<graphics::Mesh*> meshes = model->GetMeshes();
			if (meshIndex == -1)
			{
				for (graphics::Mesh* mesh : meshes)
					s_gizmoMaterial->Draw(mesh);
			}
			else
			{
				s_gizmoMaterial->Draw(meshes[meshIndex]);
			}
		}

		void Gizmos::DrawWireModel(resource::Model * model, math::Vector3 position, math::Quaternion rotation, math::Vector3 scale)
		{
			DrawWireModel(model, -1, position, rotation, scale);
		}

		void Gizmos::DrawWireModel(resource::Model * model, int meshIndex, math::Vector3 position, math::Quaternion rotation, math::Vector3 scale)
		{
			s_gizmoMaterial->SetShaderPass((int)GizmoPasses::WIREFRAME);
			s_gizmoMaterial->m_topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			s_gizmoMaterial->SetMatrix("gizmoMatrix", math::CreateMatrix(position, rotation, scale));
			s_gizmoMaterial->Bind();
			std::vector<graphics::Mesh*> meshes = model->GetMeshes();
			if (meshIndex == -1)
			{
				for (graphics::Mesh* mesh : meshes)
					s_gizmoMaterial->Draw(mesh);
			}
			else
			{
				s_gizmoMaterial->Draw(meshes[meshIndex]);
			}
		}
		

		void Gizmos::DrawCube(math::Vector3 center, math::Vector3 size)
		{
			s_gizmoMaterial->SetShaderPass((int)GizmoPasses::SOLID);
			s_gizmoMaterial->m_topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			s_gizmoMaterial->Bind();
			//s_gizmoMaterial->Draw(graphics::Model::GetPrimitive(graphics::PrimitiveType::Cube))
		}

		void Gizmos::DrawWireCube(math::Vector3 center, math::Vector3 size)
		{
			s_gizmoMaterial->SetShaderPass((int)GizmoPasses::SOLID);
			s_gizmoMaterial->m_topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			s_gizmoMaterial->Bind();
			//s_gizmoMaterial->Draw(graphics::Model::GetPrimitive(graphics::PrimitiveType::Cube))
		}

		void Gizmos::DrawBoundingOrientedBox(math::BoundingOrientedBox & obb)
		{
			math::Vector3 corners[math::BoundingOrientedBox::CORNER_COUNT];
			obb.GetCorners(corners);

			std::vector<math::Vector3> lines(24);

			lines[0] = corners[0];
			lines[1] = corners[1];
			lines[2] = corners[1];
			lines[3] = corners[2];
			lines[4] = corners[2];
			lines[5] = corners[3];
			lines[6] = corners[3];
			lines[7] = corners[0];

			lines[8] = corners[0];
			lines[9] = corners[4];
			lines[10] = corners[1];
			lines[11] = corners[5];
			lines[12] = corners[2];
			lines[13] = corners[6];
			lines[14] = corners[3];
			lines[15] = corners[7];

			lines[16] = corners[4];
			lines[17] = corners[5];
			lines[18] = corners[5];
			lines[19] = corners[6];
			lines[20] = corners[6];
			lines[21] = corners[7];
			lines[22] = corners[7];
			lines[23] = corners[4];

			ID3D11Buffer* vertexBuffer = utils::D3d::CreateBufferFromVector(lines, D3D11_BIND_VERTEX_BUFFER);

			s_gizmoMaterial->SetShaderPass((int)GizmoPasses::SOLID);

			s_gizmoMaterial->GetShader()->BindVertexBuffer(vertexBuffer, sizeof(math::Vector3));
			s_gizmoMaterial->m_topology = D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
			s_gizmoMaterial->Bind();
			s_gizmoMaterial->Draw(lines.size(), 0);
			SAFE_RELEASE(vertexBuffer);
		}

		void Gizmos::DrawBoundingSphere(math::BoundingSphere & sphere)
		{
			math::Vector3 origin = sphere.Center;
			const float radius = sphere.Radius;

			math::Vector3 xAxis = math::Vector3::UnitX * radius;
			math::Vector3 yAxis = math::Vector3::UnitY * radius;
			math::Vector3 zAxis = math::Vector3::UnitZ * radius;

			DrawRing(origin, xAxis, zAxis);
			DrawRing(origin, xAxis, yAxis);
			DrawRing(origin, yAxis, zAxis);
		}

		void Gizmos::DrawRing(math::Vector3 origin, math::Vector3 majorAxis, math::Vector3 minorAxis)
		{
			static const size_t ringSegments = 32;

			float angleDelta = math::PI*2.0f / float(ringSegments);

			std::vector<math::Vector3> lines(ringSegments + 1);

			math::Vector3 cosDelta = math::Vector3(cosf(angleDelta));
			math::Vector3 sinDelta = math::Vector3(sinf(angleDelta));
			math::Vector3 incrementalSin = math::Vector3::Zero;
			math::Vector3 incrementalCos = math::Vector3::One;
			for (size_t i = 0; i < ringSegments; i++)
			{
				math::Vector3 pos = majorAxis*incrementalCos + origin;
				pos = minorAxis*incrementalSin + pos;
				lines[i] = pos;
				// Standard formula to rotate a vector.
				math::Vector3 newCos = incrementalCos * cosDelta - incrementalSin * sinDelta;
				math::Vector3 newSin = incrementalCos * sinDelta + incrementalSin * cosDelta;
				incrementalCos = newCos;
				incrementalSin = newSin;
			}
			lines[ringSegments] = lines[0];


			ID3D11Buffer* vertexBuffer = utils::D3d::CreateBufferFromVector(lines, D3D11_BIND_VERTEX_BUFFER);

			s_gizmoMaterial->SetShaderPass((int)GizmoPasses::SOLID);

			s_gizmoMaterial->GetShader()->BindVertexBuffer(vertexBuffer, sizeof(math::Vector3));
			s_gizmoMaterial->m_topology = D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP;
			s_gizmoMaterial->Bind();
			s_gizmoMaterial->Draw(lines.size(), 0);
			SAFE_RELEASE(vertexBuffer);

		}

		void Gizmos::DrawLine(math::Vector3 from, math::Vector3 to)
		{
			std::vector<math::Vector3> corners(2);
			corners[0] = from;
			corners[1] = to;

			ID3D11Buffer* vertexBuffer = utils::D3d::CreateBufferFromVector(corners, D3D11_BIND_VERTEX_BUFFER);

			s_gizmoMaterial->SetShaderPass((int)GizmoPasses::SOLID);

			s_gizmoMaterial->GetShader()->BindVertexBuffer(vertexBuffer, sizeof(math::Vector3));
			s_gizmoMaterial->m_topology = D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
			s_gizmoMaterial->Bind();
			s_gizmoMaterial->Draw(corners.size(), 0);
			SAFE_RELEASE(vertexBuffer);
		}

		void Gizmos::DrawSphere(math::Vector3 center, float radius)
		{
			s_gizmoMaterial->SetShaderPass((int)GizmoPasses::SOLID);
			s_gizmoMaterial->m_topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			s_gizmoMaterial->Bind();
			//s_gizmoMaterial->Draw(graphics::Model::GetPrimitive(graphics::PrimitiveType::Cube))
		}

		void Gizmos::DrawWireSphere(math::Vector3 center, float radius)
		{
			s_gizmoMaterial->SetShaderPass((int)GizmoPasses::WIREFRAME);
			s_gizmoMaterial->m_topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			s_gizmoMaterial->Bind();
			//s_gizmoMaterial->Draw(graphics::Model::GetPrimitive(graphics::PrimitiveType::Cube))
		}

		void Gizmos::DrawRay(math::Vector3 from, math::Vector3 direction)
		{

			std::vector<math::Vector3> corners(2);
			corners[0] = from;
			corners[1] = from + direction * 1000;

			ID3D11Buffer* vertexBuffer = utils::D3d::CreateBufferFromVector(corners, D3D11_BIND_VERTEX_BUFFER);

			s_gizmoMaterial->SetShaderPass((int)GizmoPasses::SOLID);

			s_gizmoMaterial->GetShader()->BindVertexBuffer(vertexBuffer, sizeof(math::Vector3));
			s_gizmoMaterial->m_topology = D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
			s_gizmoMaterial->Bind();
			s_gizmoMaterial->Draw(corners.size(), 0);
			SAFE_RELEASE(vertexBuffer);
		}

		void Gizmos::DrawRay(math::Ray ray)
		{
			std::vector<math::Vector3> corners(2);
			corners[0] = ray.position;
			corners[1] = ray.position + ray.direction * 1000;

			ID3D11Buffer* vertexBuffer = utils::D3d::CreateBufferFromVector(corners, D3D11_BIND_VERTEX_BUFFER);

			s_gizmoMaterial->SetShaderPass((int)GizmoPasses::SOLID);

			s_gizmoMaterial->GetShader()->BindVertexBuffer(vertexBuffer, sizeof(math::Vector3));
			s_gizmoMaterial->m_topology = D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
			s_gizmoMaterial->Bind();
			s_gizmoMaterial->Draw(corners.size(), 0);
			SAFE_RELEASE(vertexBuffer);
		}

		void Gizmos::DrawFrustum(math::Vector3 center, float fov, float maxRange, float minRange, float aspect)
		{
			math::Matrix proj = math::Matrix::CreatePerspectiveFieldOfView(fov, aspect, minRange, maxRange);
			DrawFrustum(math::BoundingFrustum(proj));
		}

		void Gizmos::DrawFrustum(math::BoundingFrustum& frustrum)
		{
						
			math::Vector3 corners[DirectX::BoundingFrustum::CORNER_COUNT];
			frustrum.GetCorners(corners);

			std::vector<math::Vector3> lines(24);

			lines[0] = corners[0];
			lines[1] = corners[1];
			lines[2] = corners[1];
			lines[3] = corners[2];
			lines[4] = corners[2];
			lines[5] = corners[3];
			lines[6] = corners[3];
			lines[7] = corners[0];

			lines[8] = corners[0];
			lines[9] = corners[4];
			lines[10] = corners[1];
			lines[11] = corners[5];
			lines[12] = corners[2];
			lines[13] = corners[6];
			lines[14] = corners[3];
			lines[15] = corners[7];

			lines[16] = corners[4];
			lines[17] = corners[5];
			lines[18] = corners[5];
			lines[19] = corners[6];
			lines[20] = corners[6];
			lines[21] = corners[7];
			lines[22] = corners[7];
			lines[23] = corners[4];
			
			ID3D11Buffer* vertexBuffer = utils::D3d::CreateBufferFromVector(lines, D3D11_BIND_VERTEX_BUFFER);

			s_gizmoMaterial->SetShaderPass((int)GizmoPasses::SOLID);

			s_gizmoMaterial->GetShader()->BindVertexBuffer(vertexBuffer, sizeof(math::Vector3));
			s_gizmoMaterial->m_topology = D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
			s_gizmoMaterial->Bind();
			s_gizmoMaterial->Draw(lines.size(), 0);
			SAFE_RELEASE(vertexBuffer);
		}

		void Gizmos::Init()
		{
			graphics::Shader* shader = graphics::Shader::CreateShader("gizmoShader", "../Data/FXIncludes/GizmoShader.fx");
			if (shader)
			{
				s_gizmoMaterial = new graphics::Material(shader);
				SetColor(math::Color(1, 1, 1));
				SetMatrix(math::Matrix::Identity);
			}
		}

		//TODO: Needs destroy
		
		void Gizmos::SetColor(math::Color color)
		{
			s_gizmoMaterial->SetColor("gizmoColor", color);
		}

		void Gizmos::SetMatrix(math::Matrix matrix)
		{
			s_gizmoMaterial->SetMatrix("gizmoMatrix", matrix);
		}

	}
}
