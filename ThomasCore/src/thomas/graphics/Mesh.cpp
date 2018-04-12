#include "Mesh.h"
#include "../utils/d3d.h"
#include "../ThomasCore.h"
#include "../resource/Material.h"
#include "../object/component/Camera.h"
#include "../object/component/Transform.h"
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

			std::vector<utils::buffers::VertexBuffer*> vertexBuffers;

			for (auto semantic : shader->GetCurrentPass().inputSemantics)
			{
				if (m_data.vertexBuffers.find(semantic) != m_data.vertexBuffers.end())
					vertexBuffers.push_back(m_data.vertexBuffers[semantic]);
			}

			shader->BindVertexBuffers(vertexBuffers);
			if (m_data.indexBuffer)
			{
				shader->BindIndexBuffer(m_data.indexBuffer);
				thomas::ThomasCore::GetDeviceContext()->DrawIndexed(GetIndexCount(), 0, 0);
			}
			else
			{
				thomas::ThomasCore::GetDeviceContext()->Draw(m_data.vertices.positions.size(), 0);
			}

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
			if(m_data.vertices.positions.size() > 0)
				m_data.vertexBuffers[resource::Shader::Semantics::POSITION] = new utils::buffers::VertexBuffer(m_data.vertices.positions);
			if (m_data.vertices.colors.size() > 0)
				m_data.vertexBuffers[resource::Shader::Semantics::COLOR] = new utils::buffers::VertexBuffer(m_data.vertices.colors);
			if (m_data.vertices.texCoord0.size() > 0)
				m_data.vertexBuffers[resource::Shader::Semantics::TEXCOORD] = new utils::buffers::VertexBuffer(m_data.vertices.texCoord0);
			if (m_data.vertices.normals.size() > 0)
				m_data.vertexBuffers[resource::Shader::Semantics::NORMAL] = new utils::buffers::VertexBuffer(m_data.vertices.normals);
			if (m_data.vertices.tangents.size() > 0)
				m_data.vertexBuffers[resource::Shader::Semantics::TANGENT] = new utils::buffers::VertexBuffer(m_data.vertices.tangents);
			if (m_data.vertices.bitangents.size() > 0)
				m_data.vertexBuffers[resource::Shader::Semantics::BITANGENT] = new utils::buffers::VertexBuffer(m_data.vertices.bitangents);

			if(!m_data.indices.empty())
				m_data.indexBuffer = new utils::buffers::IndexBuffer(m_data.indices);
			

		}
		math::BoundingBox Mesh::GenerateBounds()
		{
			math::BoundingBox bounds;
			std::vector<math::Vector3> points(m_data.vertices.positions.size());
			for (int i = 0; i < m_data.vertices.positions.size(); i++)
			{
				points[i] = math::Vector3(m_data.vertices.positions[i]);
			}


			math::BoundingBox::CreateFromPoints(bounds, points.size(), points.data(), sizeof(math::Vector3));
			return bounds;
		}
	}
}
