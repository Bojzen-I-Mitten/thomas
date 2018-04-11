#include "Mesh.h"
#include "../utils/d3d.h"
#include "../ThomasCore.h"

namespace thomas {
	namespace graphics {

		Mesh::Mesh(Vertices vertices, std::vector<int> indices, std::string name)
		{
			m_data.vertices = vertices;
			m_data.indices = indices;
			m_name = name;
			SetupMesh();
			m_bounds = GenerateBounds();
		}

		Mesh::~Mesh()
		{
			for (auto const& buffer : m_data.vertexBuffers)
				delete buffer.second;
			delete m_data.indexBuffer;
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
			return m_data.vertices.positions.size();
		}

		void Mesh::Draw(resource::Shader * shader)
		{

			std::vector<buffers::VertexBuffer*> vertexBuffers;

			for (auto semantic : shader->GetPasses()->at(0).inputSemantics)
			{
				if (m_data.vertexBuffers.find(semantic) != m_data.vertexBuffers.end())
					vertexBuffers.push_back(m_data.vertexBuffers[semantic]);
			}

			shader->BindVertexBuffers(vertexBuffers);
			shader->BindIndexBuffer(m_data.indexBuffer->GetBuffer());
			thomas::ThomasCore::GetDeviceContext()->DrawIndexed(GetIndexCount(), 0, 0);
		}

		Vertices* Mesh::GetVertices()
		{
			return &m_data.vertices;
		}

		std::vector<int>* Mesh::GetIndices()
		{
			return &m_data.indices;
		}


		void Mesh::SetupMesh()
		{

			m_data.vertexBuffers[resource::Shader::Semantics::POSITION] = new buffers::VertexBuffer(m_data.vertices.positions);
			m_data.vertexBuffers[resource::Shader::Semantics::TEXCOORD] = new buffers::VertexBuffer(m_data.vertices.uvs);
			m_data.vertexBuffers[resource::Shader::Semantics::NORMAL] = new buffers::VertexBuffer(m_data.vertices.normals);
			m_data.vertexBuffers[resource::Shader::Semantics::TANGENT] = new buffers::VertexBuffer(m_data.vertices.tangents);
			m_data.vertexBuffers[resource::Shader::Semantics::BITANGENT] = new buffers::VertexBuffer(m_data.vertices.bitangents);

			m_data.indexBuffer = new buffers::IndexBuffer(m_data.indices);
			

		}
		math::BoundingBox Mesh::GenerateBounds()
		{
			math::BoundingBox bounds;
			std::vector<math::Vector3> points(m_data.vertices.positions.size());
			for (int i = 0; i < m_data.vertices.positions.size(); i++)
			{
				points[i] = m_data.vertices.positions[i];
			}


			math::BoundingBox::CreateFromPoints(bounds, points.size(), points.data(), sizeof(math::Vector3));
			return bounds;
		}
	}
}
