#include "Mesh.h"
#include "Shader.h"
#include "../utils/d3d.h"
#include "../ThomasCore.h"

namespace thomas {
	namespace graphics {

		Mesh::Mesh(std::vector<Vertex> vertices, std::vector<int> indices, std::string name)
		{
			m_data.vertices = vertices;
			m_data.indices = indices;
			m_name = name;
			SetupMesh();
			m_bounds = GenerateBounds();
		}

		Mesh::~Mesh()
		{
			SAFE_RELEASE(m_data.vertexBuffer);
			SAFE_RELEASE(m_data.indexBuffer);
			m_data.vertices.clear();
			m_data.indices.clear();
		}


		bool Mesh::SetName(std::string name)
		{
			m_name = name;
			return false;
		}

		MeshData* Mesh::GetData()
		{
			return &m_data;
		}

		std::string Mesh::GetName()
		{
			return m_name;
		}

		int Mesh::GetIndexCount()
		{
			return m_data.indices.size();
		}

		int Mesh::GetVertexCount()
		{
			return m_data.vertices.size();
		}

		void Mesh::Draw(Shader * shader)
		{
			shader->BindVertexBuffer(m_data.vertexBuffer, sizeof(Vertex), 0);
			shader->BindIndexBuffer(m_data.indexBuffer);
			thomas::ThomasCore::GetDeviceContext()->DrawIndexed(GetIndexCount(), 0, 0);
		}

		std::vector<Vertex>* Mesh::GetVertices()
		{
			return &m_data.vertices;
		}

		std::vector<int>* Mesh::GetIndices()
		{
			return &m_data.indices;
		}


		void Mesh::SetupMesh()
		{
			if (!m_data.vertices.empty())
			{
				m_data.vertexBuffer = utils::D3d::CreateBufferFromVector(m_data.vertices, D3D11_BIND_VERTEX_BUFFER);

				if (m_data.vertexBuffer == nullptr)
					LOG("ERROR::INITIALIZING::VERTEX::BUFFER");
			}
			else
				m_data.vertexBuffer = NULL;
			
			if (!m_data.indices.empty())
			{
				m_data.indexBuffer = utils::D3d::CreateBufferFromVector(m_data.indices, D3D11_BIND_INDEX_BUFFER);

				if (m_data.indexBuffer == nullptr)
					LOG("ERROR::INITIALIZING::INDEX::BUFFER");
			}
			else
				m_data.indexBuffer = NULL;

			

		}
		math::BoundingBox Mesh::GenerateBounds()
		{
			math::BoundingBox bounds;
			std::vector<math::Vector3> points(m_data.vertices.size());
			for (int i = 0; i < m_data.vertices.size(); i++)
			{
				points[i] = m_data.vertices[i].position;
			}


			math::BoundingBox::CreateFromPoints(bounds, points.size(), points.data(), sizeof(math::Vector3));
			return bounds;
		}
	}
}
