#include "Mesh.h"

namespace thomas 
{
	namespace graphics 
	{
		Mesh::Mesh(const Vertices & vertices, std::vector<unsigned int> indices, const std::string & name) : m_name(name)
		{
			m_data.vertices = vertices;
			m_data.indices = indices;
			SetupMesh();
			m_bounds = GenerateBounds();
		}

		void Mesh::Draw(resource::Shader * shader)
		{
			std::vector<utils::buffers::VertexBuffer*> vertexBuffers;

			//Add vertex buffers?
			for (auto semantic : shader->GetCurrentPass().inputSemantics)
			{
				if (m_data.vertexBuffers.find(semantic) != m_data.vertexBuffers.end())
					vertexBuffers.push_back(m_data.vertexBuffers[semantic].get());			
			}

			//Set buffers and draw mesh
			shader->BindVertexBuffers(vertexBuffers);
			if (m_data.indexBuffer)
			{
				shader->BindIndexBuffer(m_data.indexBuffer.get());
				thomas::ThomasCore::GetDeviceContext()->DrawIndexed(GetIndexCount(), 0, 0);
			}
			else
				thomas::ThomasCore::GetDeviceContext()->Draw(m_data.vertices.positions.size(), 0);
		}

		void Mesh::SetName(const std::string & name)
		{
			m_name = name;
		}

		MeshData & Mesh::GetData()
		{
			return m_data;
		}

		std::string Mesh::GetName() const
		{
			return m_name;
		}

		unsigned int Mesh::GetIndexCount() const
		{
			return m_data.indices.size();
		}

		unsigned int Mesh::GetVertexCount() const
		{
			return m_data.vertices.positions.size();
		}

		Vertices & Mesh::GetVertices()
		{
			return m_data.vertices;
		}

		std::vector<unsigned int> & Mesh::GetIndices()
		{
			return m_data.indices;
		}

		void Mesh::SetupMesh()
		{
			//Insert data if the property exist
			if (m_data.vertices.positions.size() > 0)
				m_data.vertexBuffers.insert(std::make_pair(resource::Shader::Semantics::POSITION, std::make_unique<utils::buffers::VertexBuffer>(m_data.vertices.positions)));
			if (m_data.vertices.colors.size() > 0)
				m_data.vertexBuffers.insert(std::make_pair(resource::Shader::Semantics::COLOR, std::make_unique<utils::buffers::VertexBuffer>(m_data.vertices.colors)));
			if (m_data.vertices.texCoord0.size() > 0)
				m_data.vertexBuffers.insert(std::make_pair(resource::Shader::Semantics::TEXCOORD, std::make_unique<utils::buffers::VertexBuffer>(m_data.vertices.texCoord0)));
			if (m_data.vertices.normals.size() > 0)
				m_data.vertexBuffers.insert(std::make_pair(resource::Shader::Semantics::NORMAL, std::make_unique<utils::buffers::VertexBuffer>(m_data.vertices.normals)));
			if (m_data.vertices.tangents.size() > 0)
				m_data.vertexBuffers.insert(std::make_pair(resource::Shader::Semantics::TANGENT, std::make_unique<utils::buffers::VertexBuffer>(m_data.vertices.tangents)));
			if (m_data.vertices.bitangents.size() > 0)
				m_data.vertexBuffers.insert(std::make_pair(resource::Shader::Semantics::BITANGENT, std::make_unique<utils::buffers::VertexBuffer>(m_data.vertices.bitangents)));

			if (!m_data.indices.empty())
				m_data.indexBuffer = std::make_unique<utils::buffers::IndexBuffer>(m_data.indices);
		}

		math::BoundingBox Mesh::GenerateBounds()
		{
			math::BoundingBox bounds;
			std::vector<math::Vector3> points(m_data.vertices.positions.size());

			for (auto point : m_data.vertices.positions)
				points.push_back(math::Vector3(point));

			math::BoundingBox::CreateFromPoints(bounds, points.size(), points.data(), sizeof(math::Vector3));
			return bounds;
		}
	}
}
